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

#include "labelFactory.h"

#include "label.h"
#include "labelProperties.h"

using namespace qReal;
using namespace qReal::gui::editor;

LabelFactory::LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi, const Id &elementId)
	: mGraphicalModelAssistApi(graphicalModelAssistApi)
	, mId(elementId)
{
}

LabelInterface *LabelFactory::createLabel(int index, qreal x, qreal y, const QString &text, qreal rotation)
{
	return new Label(mGraphicalModelAssistApi, mId, LabelProperties(index, x, y, text, rotation));
}

LabelInterface *LabelFactory::createLabel(int index, qreal x, qreal y, const QString &binding
		, const QString &roleName, const QString &nameOfRoleProperty, bool readOnly, qreal rotation)
{
	return new Label(mGraphicalModelAssistApi, mId, LabelProperties(index, x, y, binding, roleName, nameOfRoleProperty, readOnly, rotation));
}
