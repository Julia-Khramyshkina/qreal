/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>

namespace qReal {

class UmlCheckerHandler
{

public:
	UmlCheckerHandler(qrRepo::RepoApi *umlPerfectRepoApi
			, qrRepo::RepoApi *umlOrdinaryRepoApi);

	virtual ~UmlCheckerHandler();

	void addBlockName(const QString &blockName);

	void init();

	bool matchingResult();

	bool checkMatchingNodes(IdList &perfectValues, IdList &ordinaryValues);

	bool matchNodeElement(const Id &id, IdList &ordinaryValues);

	bool matchingLinksOfNode(const IdList &perfectLinks, const IdList &ordinaryLinks);

	bool matchingInsideABlock(QMultiHash<QString, Id> perfectElements
			, QMultiHash<QString, Id> &ordinaryElements);

	void saveSolution();

	QMultiHash<QString, Id> getElements(const QString &typeSolution) const;
private:
	QMultiHash<QString, Id> getElementsFromApi(qrRepo::RepoApi *repoApi) const;
	QHash<QString, QMultiHash<QString, Id>> getElementsAsBlocks(qrRepo::RepoApi *repoApi);
	QStringList initBlockNames();

	IdList doShift(const IdList &list);

	QString mQRealSourceFilesPath;
	QString mPathToPerfect;
	QString mPathToPerfectList;

	QString mPathToOrdinary;

	QStringList mBlockNames;
	QStringList mPerfectFileNames;

	qrRepo::RepoApi *mPerfectRepoFromList;

	qrRepo::RepoApi *mPerfectRepoApi;
	qrRepo::RepoApi *mOrdinaryRepoApi;
};
}