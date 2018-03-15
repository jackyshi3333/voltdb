/* This file is part of VoltDB.
 * Copyright (C) 2008-2018 VoltDB Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with VoltDB.  If not, see <http://www.gnu.org/licenses/>.
 */

package org.voltdb.sysprocs;

import org.apache.log4j.Logger;
import org.voltcore.logging.VoltLogger;
import org.voltdb.DependencyPair;
import org.voltdb.ParameterSet;
import org.voltdb.SystemProcedureExecutionContext;
import org.voltdb.VoltSystemProcedure;
import org.voltdb.VoltTable;
import org.voltdb.dtxn.DtxnConstants;
import org.voltdb.utils.VoltTableUtil;

import java.util.List;
import java.util.Map;

/**
 * Access key/value tables of cluster info that correspond to the REST
 * API members/properties
 */
public class PingAllNodes extends VoltSystemProcedure {
    private static final VoltLogger hostLog = new VoltLogger("HOST");

    static final int DEP_DISTRIBUTE = (int)
            SysProcFragmentId.PF_pingAllNodes | DtxnConstants.MULTIPARTITION_DEPENDENCY;
    static final int DEP_AGGREGATE = (int) SysProcFragmentId.PF_pingAllNodesAggregate;

    @Override
    public long[] getPlanFragmentIds() {
        return new long[]{
                SysProcFragmentId.PF_pingAllNodes,
                SysProcFragmentId.PF_pingAllNodesAggregate
        };
    }

    @Override
    public DependencyPair executePlanFragment(Map<Integer, List<VoltTable>> dependencies,
                                              long fragmentId,
                                              ParameterSet params,
                                              SystemProcedureExecutionContext context) {
        VoltTable dummy = new VoltTable(STATUS_SCHEMA);
        dummy.addRow(STATUS_OK);

        if (fragmentId == SysProcFragmentId.PF_pingAllNodes) {
            return new DependencyPair.TableDependencyPair(DEP_DISTRIBUTE, dummy);
        } else if (fragmentId == SysProcFragmentId.PF_pingAllNodesAggregate) {
            return new DependencyPair.TableDependencyPair(DEP_AGGREGATE, dummy);
        }
        assert (false);
        return null;
    }

    /**
     * Dummy Write System Proc
     *
     * @throws VoltAbortException
     */
    public VoltTable[] run(SystemProcedureExecutionContext ctx) throws VoltAbortException {
        SynthesizedPlanFragment spf[] = new SynthesizedPlanFragment[2];
        spf[0] = new SynthesizedPlanFragment();
        spf[0].fragmentId = SysProcFragmentId.PF_pingAllNodes;
        spf[0].outputDepId = DEP_DISTRIBUTE;
        spf[0].inputDepIds = new int[] {};
        spf[0].multipartition = true;
        spf[0].parameters = ParameterSet.emptyParameterSet();

        spf[1] = new SynthesizedPlanFragment();
        spf[1] = new SynthesizedPlanFragment();
        spf[1].fragmentId = SysProcFragmentId.PF_pingAllNodesAggregate;
        spf[1].outputDepId = DEP_AGGREGATE;
        spf[1].inputDepIds = new int[] { DEP_DISTRIBUTE };
        spf[1].multipartition = false;
        spf[1].parameters = ParameterSet.emptyParameterSet();
        return executeSysProcPlanFragments(spf, DEP_AGGREGATE);
    }

}