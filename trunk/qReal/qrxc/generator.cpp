/** @file generator.cpp
 * 	@brief Генератор классов используемых на диаграммах элементов
*/
#include <QtGui/QPainter>

#include "editor.h"
#include "generator.h"
#include <QMessageBox>

const int MAX_LINE_LENGTH = 60;

Generator::Generator(QString const &inf)
{
	mResources = "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n";
	mSrcDir = "";
	mRecursive = false;
	loadFile(inf);
	qDebug() << "loading files complete";
}

bool Generator::loadFile(QString const &fileName, const EditorFile **file)
{
	qDebug() << "trying to load file " << fileName << "in " << QDir(mSrcDir).absolutePath();
	EditorFile *efile = NULL;

	const EditorFile *temp = NULL, *f = NULL;
	if (!file)
		file = &temp;
	*file = NULL;

	QFileInfo const fileInfo = QFileInfo(QDir(mSrcDir), fileName);
	if (!fileInfo.exists())
	{
		if (!mRecursive) {
			mRecursive = true;
			setSrcDir("../commonXml");
			if (!loadFile(fileName, file)) {
				qDebug() << "Cannot find file" << fileName;
				return false;

			}
		} else
			return false;
		return true;
	}
	f = findFile(fileInfo);
	if (f)
	{
		if (f->isLoaded())
		{
			*file = f;
			return true; // Already loaded
		}
		else
		{
			qDebug() << "Vicious circle detected while loading file"
				<< fileInfo.canonicalFilePath();
			return false;
		}
	}

	efile = new EditorFile(fileInfo, this);
	if (!efile->load())
	{
		qDebug() << "Failed to load file " << fileInfo.canonicalFilePath();
		delete efile;
		return false;
	}
	mLoadedFiles << efile;
	*file = efile;
	return true;
}

const EditorFile* Generator::findFile(QFileInfo const &fileInfo) const
{
	Q_FOREACH (const EditorFile *f, mLoadedFiles)
		if (f->fileInfo() == fileInfo)
			return f;
	return NULL;
}

const Editor* Generator::findEditor(QString const &name) const
{
	Q_FOREACH (const EditorFile *f, mLoadedFiles)
	{
		const Editor *c = f->findEditor(name);
		if (c)
			return c;
	}
	return NULL;
}

QString Generator::normalizeName(QString const &name) const
{
	QString result = name;
	result = upperFirst(name.toLower().remove("diagram").simplified().replace(" ", "_"));
	while (result.endsWith("_"))
		result.chop(1);
	return result;
}

bool Generator::generate(QString const &outputFileName)
{
	// creating directory for generated stuff
	mDir.cd(".");
	mDir.mkdir("generated");
	if (!mDir.cd("generated"))
	{
		qDebug() << "cannot chdir() to 'generated' directory";
		return false;
	}

	QString const pluginName = normalizeName((*(mLoadedFiles.last()->constEdBegin()))->get_name());

	// generate all the stuff needed
	// Порядок важен, т.к. функции модифицируют и используют поля mHeaders и
	// mSources. TODO: ликвидировать безобразие.
	genElementClasses(pluginName);
	genPluginSource(pluginName);
	genPluginHeader(pluginName);
	genProFile(outputFileName);
	genQrcFile(outputFileName);

	qDebug() << "done";
	return true;
}

Generator::~Generator()
{
	while (!mLoadedFiles.isEmpty())
		delete mLoadedFiles.takeFirst();
}

QString Generator::upperFirst(QString const &str) const
{
	if (str.size() < 1)
		return "";

	QStringList tokens = str.split(" ");

	for (QList<QString>::iterator tokItr = tokens.begin();
		tokItr != tokens.end(); ++tokItr)
	{
		(*tokItr) = (*tokItr).at(0).toUpper() + (*tokItr).mid(1);
	}

	return tokens.join(" ");
}

void Generator::genPluginHeader(QString const &pluginName)
{
	QString fileName = pluginName + "Interface.h";
	mHeaders << fileName;

	fileName = "generated/" + fileName;

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);

	out << "#pragma once\n"
		<< "\n"
		<< "#include <QtCore/QStringList>\n"
		<< "#include <QtCore/QMap>\n"
		<< "#include <QtGui/QIcon>\n"
		<< "\n"
		<< "#include \"editorinterface.h\"\n"
		<< "\n"
		<< "class " << pluginName << "Plugin : public QObject, public qReal::EditorInterface\n"
		<< "{\n\tQ_OBJECT\n\tQ_INTERFACES(qReal::EditorInterface)\n"
		<< "\n"
		<< "public:\n"
		<< "\n"
		<< "\t" << pluginName << "Plugin();\n"
		<< "\n"
		<< "\tvirtual void initPlugin();\n"
		<< "\n"
		<< "\tvirtual QString id() const { return \"" << pluginName << "Editor\"; }\n"
		<< "\n"
		<< "\tvirtual QStringList diagrams() const;\n"
		<< "\tvirtual QStringList elements(QString const &diagram) const;\n"
		<< "\n"
		<< "\tvirtual QIcon getIcon(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual UML::Element* getGraphicalObject(QString const &diagram, QString const &element) const;\n"
		<< "\tvirtual QStringList getPropertyNames(QString const &diagram, QString const &element) const;\n"
		<< "\n"
		<< "\tvirtual QString editorName() const;\n"
		<< "\tvirtual QString diagramName(QString const &diagram) const;\n"
		<< "\tvirtual QString elementName(QString const &diagram, QString const &element) const;\n"
		<< "\n"
		<< "private:\n"
		<< "\tQMap<QString, QIcon> iconMap;\n"
		<< "\tQMap<QString, QString> diagramNameMap;\n"
		<< "\tQMap<QString, QMap<QString, QString> > elementsNameMap;\n"
		<< "};\n"
		<< "\n";

	file.close();
}

void Generator::genPluginSource(QString const &pluginName)
{
	QString fileName = pluginName + "Interface.cpp";
	mSources << fileName;

	fileName = "generated/" + fileName;

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);

	out << "#include \"" << pluginName << "Interface.h\"\n"
		<< "\n";

	foreach (QString const include, mHeaders) {
		out << "#include \"" << include << "\"\n";
	}
	out << "\n";

	out << "Q_EXPORT_PLUGIN2(qreal_editors, " << pluginName << "Plugin)\n\n"
		<< pluginName << "Plugin::" << pluginName << "Plugin(){\n"
		<< "\tinitPlugin();\n"
		<< "}\n\n"

		<< "void " << pluginName << "Plugin::initPlugin(){\n";

	FOR_ALL_FILES(f) FOR_ALL_EDITORS((*f),c){
		QString name = normalizeName((*c)->get_name());
		out << "\tdiagramNameMap[\"" << name << "\"] = \"" << name << " Diagram\"" << ";\n";
	}

	out << "\n";

	MEGA_FOR_ALL_OBJECTS(f,c,o)
	{
		if (!(*o)->visible)
			continue;
		QString ename = normalizeName((*c)->get_name());
		out << "\telementsNameMap[\"" << ename << "\"][\"" << (*o)->id << "\"] = \"" << (*o)->name << "\";\n";
	}
	out << "}\n\n"

		<< "QStringList " << pluginName << "Plugin::diagrams() const\n{\n"
		<< "\treturn diagramNameMap.keys();\n"
		<< "}\n\n"

		<< "QStringList " << pluginName << "Plugin::elements(QString const &diagram) const\n{\n"
		<< "\treturn elementsNameMap[diagram].keys();\n"
		<< "}\n\n"

		<< "QIcon " << pluginName << "Plugin::getIcon(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn QIcon(new SdfIconEngineV2(\":/\" + element + \"Class.sdf\"));\n"
		<< "}\n\n"

		<< "QString " << pluginName << "Plugin::editorName() const\n{\n"
		<< "\t return \"" << pluginName << " Editor\";\n"
		<< "}\n\n"

		<< "QString " << pluginName << "Plugin::diagramName(QString const &diagram) const\n{\n"
		<< "\treturn diagramNameMap[diagram];\n"
		<< "}\n\n"

		<< "QString " << pluginName << "Plugin::elementName(QString const &diagram, QString const &element) const\n{\n"
		<< "\treturn elementsNameMap[diagram][element];\n"
		<< "}\n\n"

		<< "UML::Element* " << pluginName << "Plugin::getGraphicalObject(QString const &diagram, QString const &element) const\n{\n";

	bool isFirst = true;

	MEGA_FOR_ALL_OBJECTS(f,c,o)
	{
		if ((*o)->type == NODE && !(*o)->visible)
			continue;
		if (isFirst) {
			out << "\tif (element == \"" << (*o)->id << "\")\n";
			isFirst = false;
		}
		else
			out << "\telse if (element == \"" << (*o)->id << "\")\n";

		out << "\t\treturn new UML::" << upperFirst((*o)->id) << "();\n";
	}
	out << "	else {\n"
		<< "		Q_ASSERT(!\"Request for creation of an element with unknown name\");\n"
		<< "		return NULL;\n"
		<< "	}\n";
	out << "}\n\n";

	out << "QStringList " << pluginName << "Plugin::getPropertyNames(QString const &diagram, QString const &element) const\n"
		<< "{\n"
		<< "\tQStringList result;\n";

	isFirst = true;
	MEGA_FOR_ALL_OBJECTS(f,c,o)
	{
		if ((*o)->type == NODE && !(*o)->visible)
			continue;
		bool isFirstProperty = true;
		if (isFirst)
		{
			out << "\tif (element == \"" << (*o)->id << "\")\n";
			isFirst = false;
		} else
			out << "\telse if (element == \"" << (*o)->id << "\")\n";

		QString props = "";
		FOR_ALL_PROPERTIES((*o),p)
		{
			if (isFirstProperty){
				out << "\t\tresult ";
				isFirstProperty = false;
			}
			props += QString(" << \"" +(*p)->getName() + "\"");
			if( props.length() >= MAX_LINE_LENGTH ){
				out << props;
				props = "\n\t\t";
			}
		}
		if( !props.trimmed().isEmpty())
			out << props;
		out << ";\n";
		isFirst = false;
	}
	out << "\treturn result;\n"
		<< "}\n";

	file.close();
}

void Generator::genElementClasses(QString const &pluginName)
{
	MEGA_FOR_ALL_OBJECTS(f,c,o)
	{
		if ((*o)->type == NODE) {
			if (!(*o)->visible)
				continue;
			genNodeClass(static_cast<Node*>(*o), pluginName);
		}
		else
			genEdgeClass(static_cast<Edge*>(*o), pluginName);
	}
}

void Generator::genNodeClass(Node *node, QString const &pluginName)
{
	QString const classname = node->id;
	QString const uClassname = upperFirst(classname);

	QString fileName = classname + ".h";
	mHeaders << fileName;

	fileName = "generated/" + fileName;
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);

	out << "#pragma once\n\n"
		<< "#include \"../../qrgui/umllib/uml_nodeelement.h\"\n\n"
		<< "namespace UML {\n\n"
		<< "\tclass " << uClassname << " : public NodeElement {\n"
		<< "\tpublic:\n"
		<< "\t\t" << uClassname << "() {\n";

	bool hasSdf, hasPorts = hasSdf = false;

	QFile sdffile("generated/shapes/" + classname + "Class.sdf");
	if (sdffile.exists()) {
		out << "\t\t\tmRenderer.load(QString(\":/" << classname << "Class.sdf\"));\n";
		hasSdf = true;
	}

	sdffile.setFileName("generated/shapes/" + classname + "Ports.sdf");
	if (sdffile.exists()) {
		out << "\t\t\tmPortRenderer.load(QString(\":/" << classname << "Ports.sdf\"));\n";
		hasPorts = true;
	}

	out << "\t\t\tm_contents.setWidth(" << node->width << ");\n"
		<< "\t\t\tm_contents.setHeight(" << node->height << ");\n"
		<< "\t\t}\n\n"

		<< "\t\t~" << uClassname << "() {}\n\n"

		<< "\t\tvoid paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)\n\t\t{\n";

	if (hasSdf)
		out << "\t\t\tmRenderer.render(painter, m_contents);\n";

	if (hasPorts)
		out << "\t\t\tNodeElement::paint(painter, style, widget, &mPortRenderer);\n";
	else
		out << "\t\t\tNodeElement::paint(painter, style, widget, NULL);\n";

	out << "\t\t}\n\n"

		<< "\t\tvoid updateData()\n\t\t{\n"
		<< "\t\t\tNodeElement::updateData();\n"
		<< "\t\t\tupdate();\n"
		<< "\t\t}\n\n"

		<< "\tprivate:\n";
	if (hasSdf)
		out << "\t\tSdfRenderer mRenderer;\n";
	if (hasPorts)
		out << "\t\tSdfRenderer mPortRenderer;\n";
	out << "\t};"
		<< "}\n";
}

void Generator::genEdgeClass(Edge *edge, QString const &/*pluginName*/)
{
	QString classname = edge->id;
	QString uClassname = upperFirst(classname);

	QString fileName = classname + ".h";
	mHeaders << fileName;

	fileName = "generated/" + fileName;

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream out(&file);

	out << "#pragma once\n\n"
		<< "#include \"../../qrgui/umllib/uml_edgeelement.h\"\n\n"
		<< "namespace UML {\n\n"
		<< "\tclass " << uClassname << " : public EdgeElement {\n"
		<< "\tpublic:\n"
		<< "\t\t" << uClassname << "() {\n"
		<< "\t\t\tm_penStyle = Qt::SolidLine;\n"
		<< "\t\t}\n\n"

		<< "\t\tvirtual ~" << uClassname << "() {}\n\n"

		<< "\tprotected:\n"
		<< "\t\tvirtual void drawStartArrow(QPainter * p) const {}\n"
		<< "\t\tvirtual void drawEndArrow(QPainter * p) const {}\n"
		<< "\t};\n"
		<< "}\n";
}

void Generator::addResource(QString const &resourceXml) {
	mResources += resourceXml;
}

void Generator::genProFile(QString const &plugin) const
{
	QFile outFile(plugin);
	outFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&outFile);

	out << "message(\"Building generated editor " + QFileInfo(outFile).fileName() + "\")" << '\n'
		<< "\n"
		<< "QRXML_ROOT = ../..\n"
		<< "\n"
		<< "!include (../../editorsSdk.pri) {\n"
		<< "	message(\"editorsSdk.pri not found at ../..\")\n"
		<< "}\n"
		<< "\n"
		<< "QT += xml\n"
		<< "\n"
		<< "TEMPLATE =  lib\n"
		<< "CONFIG += plugin\n"
		<< "DESTDIR = ../../../qrgui/plugins/\n"
		<< "INCLUDEPATH += ../..\n"
		<< "\n"
		<< "OBJECTS_DIR = ../../obj/\n"
		<< "MOC_DIR = ../../moc/\n"
		<< "\n"
		<< "include (editorsSdk.pri)\n"
		<< "\n"
		<< "HEADERS += \\\n";
	printFiles(mHeaders, out);

	out << "SOURCES += \\\n";
	printFiles(mSources, out);

	out << "\n"
		<< "RESOURCES += shapes/" + QFileInfo(plugin).baseName() + ".qrc\n";

	outFile.close();
}

void Generator::genQrcFile(QString const &plugin) const
{
	QFile outFile(QFileInfo(plugin).absolutePath() + "/shapes/" + QFileInfo(plugin).baseName() + ".qrc");
	outFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&outFile);

	out << mResources
		<< "</qresource>\n"
		<< "</RCC>\n";

	outFile.close();
}

void Generator::printFiles(QStringList const &files, QTextStream &out) {
	foreach (QString const file, files) {
		out << "	" + file + "\\\n";
	}
	out << "\n";
}
