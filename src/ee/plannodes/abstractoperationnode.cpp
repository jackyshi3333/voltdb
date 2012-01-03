/* This file is part of VoltDB.
 * Copyright (C) 2008-2012 VoltDB Inc.
 *
 * This file contains original code and/or modifications of original code.
 * Any modifications made by VoltDB Inc. are licensed under the following
 * terms and conditions:
 *
 * VoltDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * VoltDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with VoltDB.  If not, see <http://www.gnu.org/licenses/>.
 */
/* Copyright (C) 2008 by H-Store Project
 * Brown University
 * Massachusetts Institute of Technology
 * Yale University
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdexcept>
#include <sstream>
#include "abstractoperationnode.h"
#include "common/debuglog.h"
#include "common/serializeio.h"
#include "storage/table.h"
#include "catalog/table.h"
#include "json_spirit/json_spirit.h"

using namespace std;
using namespace voltdb;

AbstractOperationPlanNode::~AbstractOperationPlanNode()
{
    delete getOutputTable();
    setOutputTable(NULL);
}

string AbstractOperationPlanNode::getTargetTableName() const {
    return target_table_name;
}
Table* AbstractOperationPlanNode::getTargetTable() const {
    return (this->target_table);
}
void AbstractOperationPlanNode::setTargetTable(Table* table) {
    this->target_table = table;
}
void AbstractOperationPlanNode::setTargetTableName(string name) {
    this->target_table_name = name;
}


string AbstractOperationPlanNode::debugInfo(const string &spacer) const {
    ostringstream buffer;
    buffer << spacer << "TargetTable[" << this->target_table_name << "]\n";
    return (buffer.str());
}

void AbstractOperationPlanNode::loadFromJSONObject(json_spirit::Object &obj) {
    json_spirit::Value targetTableNameValue = json_spirit::find_value( obj, "TARGET_TABLE_NAME");
    if (targetTableNameValue == json_spirit::Value::null) {
        throw SerializableEEException(VOLT_EE_EXCEPTION_TYPE_EEEXCEPTION,
                                      "AbstractOperationPlanNode::"
                                      "loadFromJSONObject: "
                                      "Couldn't find TARGET_TABLE_NAME value");
    }
    target_table_name = targetTableNameValue.get_str();
}
