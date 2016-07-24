#ifndef LUCKY_GLPK_H
#define LUCKY_GLPK_H
//g++ -std=c++11 testglpk.cpp -ldl
#include <iostream>
#include <glpk.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

/// glpkcall 
//  Call the dynamic lib of GLPK
//  Wrap it as the like of GLPK
///
class glpkCall
{

public:
    glpkCall(const char* symbol,  int mode = RTLD_LAZY);
   
    ~glpkCall()
    {
        dlclose(pHandleDlopen);
    }
public:
    glp_prob *glp_create_prob() { return pCreateLp(); }

    void glp_delete_prob( glp_prob* lp ) { pDeleteLp(lp); }

    void glp_set_prob_name(glp_prob* lp, const char* name) { pSetName(lp, name); }

    int glp_read_lp( glp_prob* lp, const glp_cpxcp* param, const char *fname)
    { 
        return pReadLp( lp, param, fname);
    }

    int glp_simplex(glp_prob *lp, const glp_smcp *param = NULL){ return pResolveLp( lp, param ); }

    double glp_get_col_prim( glp_prob* lp, int i ){ return pGetResult( lp, i ); }
    int glp_get_status(glp_prob* lp){ return pGetStatus(lp); }

    int glp_exact(glp_prob *lp, const glp_smcp* param = NULL)
    {
        return pSolveExact(lp, param);
    }
    int glp_get_prim_stat(glp_prob *p)
    {
        return pGetPrimeStatus(p);
    }
    int glp_get_dual_stat(glp_prob *p)
    {
        return pGetDualStatus(p);
    }
    void glp_init_smcp(glp_smcp* param)
    {
        pInitSmcp(param);
    }

    int glp_read_mip(glp_prob* p, const char *fname)
    {
        return pReadMip(p,fname);
    }
    int glp_mip_status(glp_prob *p)
    {
        return pMipStatus(p);
    }
    int glp_intopt(glp_prob *p, const glp_iocp *param)
    {
        return pMipSolver(p, param);
    }
    void glp_init_iocp(glp_iocp* param){ pInitIocp(param); }

public:
    typedef glp_prob* (*create_lp)();
    typedef void (*delete_lp)(glp_prob*);
    typedef void (*set_name)( glp_prob*, const char* );
    typedef int (*read_lp)( glp_prob*, const glp_cpxcp*, const char*);
    typedef int (*resolve_lp)(glp_prob *, const glp_smcp*);
    typedef double (*get_result)(glp_prob*, int);
    typedef int (*get_status)(glp_prob*);
    typedef int (*sovle_exact)(glp_prob*, const glp_smcp*);
    typedef int (*get_prim_stat)(glp_prob*);
    typedef int (*get_dual_stat)(glp_prob*);
    typedef void (*init_smcp)(glp_smcp*);
    typedef int (*read_mip)( glp_prob*, const char *);
    typedef int (*mip_status)(glp_prob*);
    typedef int (*mip_solver)(glp_prob*, const glp_iocp*);
    typedef void (*init_iocp)(glp_iocp*);
private:
    void *pHandleDlopen;

private:
    create_lp   pCreateLp;
    delete_lp   pDeleteLp;
    set_name    pSetName;
    read_lp     pReadLp;
    resolve_lp  pResolveLp;
    get_result  pGetResult;  
    get_status  pGetStatus;
    sovle_exact pSolveExact;
    get_prim_stat pGetPrimeStatus;
    get_dual_stat pGetDualStatus;
    init_smcp   pInitSmcp;
    read_mip    pReadMip;
    mip_status  pMipStatus;
    mip_solver  pMipSolver;
    init_iocp   pInitIocp;

};

glpkCall::glpkCall(const char* symbol,  int mode )
{
    pHandleDlopen = dlopen( symbol, mode );

    if(pHandleDlopen == NULL)
    {
        std::cerr << "Can not open library: " << symbol << "\n";
        exit(1);
    }

    dlerror();

    /* pCreateLp   =  dlsym(pHandleDlopen, "glp_create_prob"); */
    pCreateLp   = (create_lp) dlsym(pHandleDlopen, "glp_create_prob");
    pSetName    = (set_name) dlsym(pHandleDlopen, "glp_set_prob_name");
    pReadLp     = (read_lp) dlsym(pHandleDlopen, "glp_read_lp");
    pResolveLp  = (resolve_lp) dlsym(pHandleDlopen, "glp_simplex");
    pGetResult  = (get_result) dlsym(pHandleDlopen, "glp_get_col_prim");
    pDeleteLp   = (delete_lp) dlsym(pHandleDlopen, "glp_delete_prob");
    pGetStatus  = (get_status) dlsym(pHandleDlopen, "glpk_get_status");
    pSolveExact = (sovle_exact) dlsym(pHandleDlopen, "glp_exact");
    pGetPrimeStatus = (get_prim_stat) dlsym(pHandleDlopen, "glp_get_prim_stat");
    pGetDualStatus  = (get_dual_stat) dlsym(pHandleDlopen, "glp_get_dual_stat");
    pInitSmcp   = (init_smcp) dlsym(pHandleDlopen, "glp_init_smcp");
    pReadMip    = (read_mip) dlsym(pHandleDlopen, "glp_read_mip");
    pMipStatus  = (mip_status) dlsym(pHandleDlopen, "glp_mip_status");
    pMipSolver  = (mip_solver) dlsym(pHandleDlopen, "glp_intopt");
    pInitIocp   = (init_iocp) dlsym(pHandleDlopen, "glp_init_iocp");
}


#endif
