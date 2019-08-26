/*
 *    This file is part of acados.
 *
 *    acados is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    acados is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with acados; if not, write to the Free Software Foundation,
 *    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

// standard
#include <stdio.h>
#include <stdlib.h>
// acados
#include "acados/utils/print.h"
#include "acados_c/ocp_nlp_interface.h"
#include "acados_c/external_function_interface.h"
#include "acados_solver_crazyflie_simple_model.h"

// ** global data **
ocp_nlp_in * nlp_in;
ocp_nlp_out * nlp_out;
ocp_nlp_solver * nlp_solver;
void * nlp_opts;
ocp_nlp_plan * nlp_solver_plan;
ocp_nlp_config * nlp_config;
ocp_nlp_dims * nlp_dims;


external_function_casadi * forw_vde_casadi;






int main() {

    int status = 0;
    status = acados_create();

    if (status) { 
        printf("acados_create() returned status %d. Exiting.\n", status); 
        exit(1); }

    // set initial condition
    double x0[9];
    
    x0[0] = 0.1;
    
    x0[1] = 0.1;
    
    x0[2] = 0.0;
    
    x0[3] = 0.0;
    
    x0[4] = 0.0;
    
    x0[5] = 0.0;
    
    x0[6] = 0.0;
    
    x0[7] = 0.0;
    
    x0[8] = 0.0;
    
    
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "lbx", x0);
    ocp_nlp_constraints_model_set(nlp_config, nlp_dims, nlp_in, 0, "ubx", x0);

    
    
    

    double kkt_norm_inf = 1e12, elapsed_time;

#if 1
    int NTIMINGS = 100;
    double min_time = 1e12;
    for (int ii = 0; ii < NTIMINGS; ii ++) {
        for (int i = 0; i <= nlp_dims->N; ++i) {
            blasfeo_dvecse(nlp_dims->nu[i]+nlp_dims->nx[i], 0.0, nlp_out->ux+i, 0);
        }
        status = acados_solve();
        elapsed_time = nlp_out->total_time;
        if (elapsed_time < min_time) min_time = elapsed_time;
    }
    elapsed_time = min_time;
#else
    status = acados_solve();
#endif
    kkt_norm_inf = nlp_out->inf_norm_res;
    elapsed_time = nlp_out->total_time;
    printf(" iterations %2d | time  %f |  KKT %e\n", nlp_out->sqp_iter, elapsed_time, kkt_norm_inf);

    printf("\n--- solution ---\n");
    ocp_nlp_out_print(nlp_solver->dims, nlp_out);
    if (status) { 
        printf("acados_solve() returned status %d.\n", status); 
    }

    status = acados_free();

    if (status) { 
        printf("acados_free() returned status %d. \n", status); 
    }

    return status;
}