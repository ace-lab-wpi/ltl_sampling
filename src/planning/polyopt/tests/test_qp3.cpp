/*
 * test_qp2.cpp
 *
 *  Created on: Aug 28, 2016
 *      Author: rdu
 */

#include <iostream>
#include <cstdint>
#include <limits>
#include <cstring>
#include <vector>
#include <ctime>

#include "gurobi_c++.h"

#include "eigen3/Eigen/Core"

#include "polyopt/polyopt_math.h"
#include "polyopt/gurobi_utils.h"

using namespace std;
using namespace librav;
using namespace Eigen;

int main(int   argc, char *argv[])
{
	clock_t exec_time;
	exec_time = clock();

	uint32_t r = 2;
	uint32_t N = 2 * r - 1;

	uint8_t kf_num = 4;

	MatrixXf Q = MatrixXf::Zero((kf_num - 1) * (N+1), (kf_num - 1) * (N+1));
	MatrixXf A_eq = MatrixXf::Zero((kf_num - 1) * 2 * r, (kf_num - 1) * (N + 1));
	MatrixXf b_eq = MatrixXf::Zero((kf_num - 1) * 2 * r, 1);

	MatrixXf keyframe_vals = MatrixXf::Zero(r, kf_num);
	MatrixXf keyframe_ts = MatrixXf::Zero(1, kf_num);

	keyframe_vals(0,0) = -0.15;
	keyframe_vals(0,1) = 0.25;
	keyframe_vals(0,2) = 0.3;
	keyframe_vals(0,3) = 0.35;

	keyframe_vals(1,0) = 0;
	keyframe_vals(1,1) = std::numeric_limits<float>::infinity();
	keyframe_vals(1,2) = std::numeric_limits<float>::infinity();
	keyframe_vals(1,3) = 0;

	keyframe_ts(0,0) = 0;
	keyframe_ts(0,1) = 1.2;
	keyframe_ts(0,2) = 3;
	keyframe_ts(0,3) = 4.5;

	PolyOptMath::GetNonDimQMatrices(N,r,kf_num, keyframe_ts,Q);
	PolyOptMath::GetNonDimEqualityConstrs(N, r, kf_num, keyframe_vals, keyframe_ts, A_eq, b_eq);

//	std::cout << "\nQ: \n" << Q << std::endl;
//	std::cout << "\nA_eq:\n" << A_eq << std::endl;
//	std::cout << "\nb_eq:\n" << b_eq << std::endl;

	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		// Create variables
		std::vector<GRBVar> sig;
		uint32_t var_num = (N + 1)*(kf_num-1);
		sig.resize(var_num);
		for(int i = 0; i < var_num; i++)
		{
			std::string var_name = "sig"+std::to_string(i);
			sig[i] = model.addVar(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0.0, GRB_CONTINUOUS, var_name);
		}

		// Integrate new variables
		model.update();

		// Set objective
		GRBQuadExpr cost_fun;
		GurobiUtils::GetQuadraticCostFuncExpr(sig, Q, var_num, cost_fun);;
		model.setObjective(cost_fun);

		// Add constraints
		GurobiUtils::AddLinEqualityConstrExpr(sig, A_eq, b_eq, var_num, var_num, model);

		// Optimize model
		model.optimize();

		exec_time = clock() - exec_time;
		std::cout << "Optimization finished in " << double(exec_time)/CLOCKS_PER_SEC << " s.\n" << std::endl;

		for(int i = 0; i < var_num; i++)
		{
			std::cout << sig[i].get(GRB_StringAttr_VarName) << " : "
					<< sig[i].get(GRB_DoubleAttr_X) << std::endl;
		}

		std::cout << "\nObj: " << model.get(GRB_DoubleAttr_ObjVal) << std::endl;

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
	}

	return 0;
}


