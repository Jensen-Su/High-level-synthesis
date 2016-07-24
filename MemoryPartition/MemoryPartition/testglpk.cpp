#include<glpk.h>
#include "myGLPK.h"
#include <iostream>
#include <stdlib.h>
#include <string>
int main()
{
    glp_prob *lp;
    glp_smcp param;
    glp_iocp iocp;

//    param->meth = GLP_DUAL;
    /* param.msg_lev = GLP_MSG_ALL; */
    /* param.meth = GLP_PRIMAL; */
    /* param.tol_bnd = 1e-7; */
    std::string libname = "/usr/local/lib/libglpk.so";
    glpkCall* glpkObj = new glpkCall(libname.c_str(), RTLD_LAZY);

    glpkObj->glp_init_smcp(&param);
    glpkObj->glp_init_iocp(&iocp);
    lp = glpkObj->glp_create_prob();
    glpkObj->glp_set_prob_name(lp, "glpk_test");

    std::string filename = "simplelp.txt";
    int lpresult;
    if(!(glpkObj->glp_read_lp( lp, NULL, filename.c_str() ) ) )
    {
        // Optimal basis to initial LP relaxation must be provided
        glpkObj->glp_simplex( lp, &param);
        // Then use branch and bound to find optimal solution
        lpresult = glpkObj->glp_intopt( lp, &iocp);
    }
    else
    {
        std::cerr << "GLPK tool cannot read in lp problem file: " << filename << "\n";
        exit(0);
    }

    std::cout << "glpk_simplex() return: " << lpresult << "\n";
    std::cout << "x = " << glpkObj->glp_get_col_prim(lp, 1) << "\n";
    std::cout << "y = " << glpkObj->glp_get_col_prim(lp, 2) << "\n";


    std::cout << "glpk_get_prime_status() return: \n ";
    int status = glpkObj->glp_get_prim_stat(lp);
    std::cout << status << "\n";
    std::cout << "GLP_INFEAS = " << GLP_INFEAS << "\n";
    std::cout << "GLP_FEAS = " << GLP_FEAS << "\n";
    // to see if there exits feasible solution
    std::cout << glpkObj->glp_mip_status(lp) << "\n";
    std::cout << "GLP_NOFEAS = " << GLP_NOFEAS << "\n";
    delete glpkObj;
    return 0;
}

