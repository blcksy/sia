/*
    featsat.c
    
    C Extension module for Python, interfacing stadistical feature computing
    utilities
    
*/

// Main Libraries
#include <Python.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

/*
Graph Features Computation for SAT instances.

Version 2.2
Authors:
  - Carlos Ansótegui (DIEI - UdL)
  - María Luisa Bonet (LSI - UPC)
  - Jesús Giráldez-Cru (IIIA-CSIC)
  - Jordi Levy (IIIA-CSIC)

Contact: jgiraldez@iiia.csic.es

    Copyright (C) 2014  C. Ansótegui, M.L. Bonet, J. Giráldez-Cru, J. Levy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// Third party libraries
#include <vector>
#ifndef VECTOR
#include "graph_set.h"
#else
#include "graph_vector.h"
#endif
#include "tools.h"
#include "powerlaw.h"
#include "dimension.h"
#include "community.h"

// Added for Windows
#ifdef _WIN32
#include <time.h>
#endif



using namespace std;


// Global variables and Flags


// Extra info. Debugging purposes
bool verbose = false;

// Execution tunning variables 
int minx = 0;
int maxx = 15;
int maxx2 = 6;
int maxclause = 400;
int maxxmin = 10;
double precision = 0.000001;

// Scale Free (Vars)
char *var = NULL;
char *alphavar = NULL;
char *varint = NULL;
char *varplot = NULL;

// Scale Free (Clauses)
char *clau = NULL;
char *alphaclau = NULL;
char *clauint = NULL;
char *clauplot = NULL;

// Self Similarity (VIG)
char *dim = NULL;
char *resdim = NULL;
char *dimplot = NULL;

// Self Similarity (CVIG)
char *dib = NULL;
char *resdib =  NULL;
char *dibplot = NULL;

// Plotting Variables (mantained, not used)
char *mod = NULL;
char *mob = NULL;
char *fout = NULL;
char *modules = NULL;


double modularity_vig(char* fin, int max_clauses) {

    clock_t t_ini, t_fin;
    double secsGraphs;
    double secsMod;

    Graph* vig = NULL;

    // Build Graph
    t_ini = clock();
    vig = readVIG(fin, max_clauses);
    t_fin = clock();
    secsGraphs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    Community c(vig);
    double modularity=-1;

    if(verbose) {
        cerr << "Computing COMMUNITY Structure (VIG)" << endl;
        std::cout << "max_clauses: " << max_clauses << std::endl;
    }

    // Computation
    t_ini = clock();
    modularity = c.compute_modularity_GFA(precision);
    c.compute_communities();
    t_fin = clock();
    secsMod = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    if (verbose) {
        cerr << "modularity = " << modularity << endl;
        cerr << "communities = " << (int)c.ncomm << endl;
        cerr << "largest size = " << (double)c.Comm[c.Comm_order[0].first].size()/vig->size() << endl;
        cerr << "iterations = " << c.iterations << endl;
        cerr << "grap loading = " << secsGraphs << endl;
        cerr << "partition and modu time = " << secsMod << endl;
    }

    return modularity;
}


double modularity_cvig(char* fin, int max_clauses) {

    // Time variables
    clock_t t_ini, t_fin;
    double secsGraphs;
    double secsModBip;

    Graph* cvig = NULL;
    
    // Build Graph
    t_ini = clock();
    cvig = readCVIG(fin, max_clauses);
    t_fin = clock();
    
    secsGraphs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    // Community
    Community c_bip(cvig);
    double modularity_bip = -1;

    // Compute
    t_ini = clock();    
    modularity_bip = c_bip.compute_modularity_GFA(precision);
    c_bip.compute_communities();
    t_fin = clock();

    secsModBip = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    if (verbose) {
        cerr << "modularity = " << modularity_bip << endl;
        cerr << "communities = " << (int)c_bip.ncomm << endl;
        cerr << "largest size = " << (double)c_bip.Comm[c_bip.Comm_order[0].first].size()/cvig->size() << endl;
        cerr << "iterations = " << c_bip.iterations << endl;
        cerr << "grap loading = " << secsGraphs << endl;
        cerr << "partition and modu time = " << secsModBip << endl;
    }
        
    return modularity_bip;
}

double scale_free_var(char* fin, int max_clauses){

    // Time variables
    clock_t t_ini, t_fin;
    double secsAlphaVar;
    
    // Scale Free (Variables)
    double alphavarexp = -1;
  
    if (verbose) {
        cerr << "Computing SCALE-FREE Structure (Variables)" << endl;
        cerr << "max_clauses: " << max_clauses << endl;
    }

    // Compute
    t_ini = clock();
    vector<pair <int,int> > a = arityVar(fin);
    alphavarexp = mostlikely(a, maxxmin, alphavar, varint, varplot, true);
    t_fin = clock();
    
    secsAlphaVar = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    
    if (verbose) {
        cerr << "alpha var exp = " << alphavarexp << endl;
        cerr << "time = " << secsAlphaVar << endl;
    }

    return alphavarexp;
}

double scale_free_clause(char* fin, int max_clauses){

    // Time variables
    clock_t t_ini, t_fin;
    double secsAlphaClau;
    
    // Scale Free (Clauses)
    double alphaclauexp = -1;

    if (verbose) {
        cerr << "Computing SCALE-FREE Structure (Clauses)" << endl;
    }

    // Compute
    t_ini = clock();
    vector<pair <int,int> > b = arityClause(fin);
    alphaclauexp = mostlikely(b, maxxmin, alphaclau, clauint, clauplot, false);
    t_fin = clock();
    
    secsAlphaClau = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    
    if (verbose) {
        cerr << "alpha clause exp = " << alphaclauexp << endl;
        cerr << "time = " << secsAlphaClau << endl;
    }

    return alphaclauexp;
}

double self_similar_vig(char* fin, int max_clauses){

    // Time management
    clock_t t_ini, t_fin;
    double secsGraphs;
    double secsDim;
    
    // Graphs
    Graph* vig = NULL;

    // Build Graph
    t_ini = clock();
    vig = readVIG(fin, max_clauses);
    t_fin = clock();
    
    secsGraphs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    // Self Similarity variables
    vector<int> needed;
    vector <pair <double,double> > v1;
    vector <pair <double,double> > v2;
    
    pair <double,double> polreg = make_pair(-1,-1);
    pair <double,double> expreg = make_pair(-1,-1);
    
    if(verbose){
        cerr << "Computing SELF-SIMILAR Structure (VIG)" << endl;
        cerr << "max_clauses: " << max_clauses << endl;
    }

    v1.clear(); 
    v2.clear();
   
    // Compute
    t_ini = clock();    
    needed = computeNeeded(vig);
    for(int i=1; i<needed.size(); i++){
        if(i>=minx && i<=maxx2){
            v1.push_back(pair<double,double>(log(i), log(needed[i])));
            v2.push_back(pair<double,double>((double)i, log(needed[i])));   
        }
    }
    polreg = regresion(v1);
    expreg = regresion(v2);
    t_fin = clock();

    secsDim = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    if (verbose) {
        cerr << "dimension = " << -polreg.first << endl;
        cerr << "decay = " << -expreg.first << endl;
    }
    
    return -polreg.first;
}

double self_similar_cvig(char* fin, int max_clauses){

    // Time management
    clock_t t_ini, t_fin;
    double secsGraphs;
    double secsDib;
    
    Graph* cvig = NULL;    
    
    // Build Graph
    t_ini = clock();
    cvig = readCVIG(fin, max_clauses);
    t_fin = clock();
    secsGraphs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    
    // Self similarity
    vector<int> needed;
    vector <pair <double,double> > v1;
    vector <pair <double,double> > v2;
    
    pair <double,double> polregB = make_pair(-1,-1);
    pair <double,double> expregB = make_pair(-1,-1);
       
    if(verbose) {
        cerr << "Computing SELF-SIMILAR Structure (CVIG)" << endl;
        cerr << "max_clauses: " << max_clauses << std::endl;
    }

    v1.clear(); 
    v2.clear();
    
    t_ini = clock();        
    needed = computeNeeded(cvig);

    for(int i=1; i<needed.size(); i++){
        if(i>=minx && i<=maxx2){
            v1.push_back(pair<double,double>(log(i), log(needed[i])));
            v2.push_back(pair<double,double>((double)i, log(needed[i])));   
        }
    }
    
    polregB = regresion(v1);
    expregB = regresion(v2);
    
    t_fin = clock();
    secsDib = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; 
    
    if (verbose) {
        cerr << "dimension bipartite = " << -polregB.first << endl;
        cerr << "decay bipartite = " << -expregB.first << endl;
    }

    return -polregB.first;
}

// Test purposes
double compute_all(char* fin, int max_clauses) {

	clock_t t_ini, t_fin;
    double secsGraphs, secsTotal;
	double secsAlphaVar, secsAlphaClau;
	double secsDim, secsDib;
	double secsMod, secsModBip;

    t_ini = clock();
    
    Graph* vig = NULL;
    Graph* cvig = NULL;

    // Maxclauses
    cout << "max_clause: " << max_clauses << endl;
    
    t_ini = clock();
    pair<Graph*,Graph*> p = readFormula(fin, max_clauses);
    t_fin = clock();
    secsGraphs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    // Graphs
    vig = p.first;
    cvig = p.second;

    
    // Scale Free
    double alphavarexp = -1;
    double alphaclauexp = -1;

    // Self similarity
    vector<int> needed;
    vector <pair <double,double> > v1;
    vector <pair <double,double> > v2;
    pair <double,double> polreg  = make_pair(-1,-1);
    pair <double,double> expreg  = make_pair(-1,-1);
    pair <double,double> polregB = make_pair(-1,-1);
    pair <double,double> expregB = make_pair(-1,-1);

    // Modularity
    Community c(vig);
    Community c_bip(cvig);
    double modularity=-1;
    double modularity_bip=-1;
    
    //**************************************************************************
    // Scale Free (Variables)
    //**************************************************************************

    if(verbose) {
        cerr << "Computing SCALE-FREE Structure (Variables)" << endl;
        cerr << "max_clauses: " << max_clauses << std::endl;
    }

    t_ini = clock();
    vector<pair <int,int> > a = arityVar(fin);
    alphavarexp = mostlikely(a, maxxmin, alphavar, varint, varplot, true);
    t_fin = clock();
    secsAlphaVar = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    
    //**************************************************************************
    // Scale Free (Clauses)
    //**************************************************************************
   
    if(verbose) {
       cerr << "Computing SCALE-FREE Structure (Clauses)" << endl;
       cerr << "max_clauses: " << max_clauses << endl;
    }

    t_ini = clock();
    vector<pair <int,int> > b = arityClause(fin);
    alphaclauexp = mostlikely(b, maxxmin, alphaclau, clauint, clauplot, false);
    t_fin = clock();
    secsAlphaClau = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    
    //**************************************************************************
    // Computing SELF-SIMILAR Structure (VIG)
	//**************************************************************************          
    
    if(verbose){
        cerr << "Computing SELF-SIMILAR Structure (VIG)" << endl;
        cerr << "max_clauses: " << max_clauses << endl;
    }

    v1.clear(); v2.clear();
    
    t_ini = clock();    
    needed = computeNeeded(vig);
    
    for(int i=1; i<needed.size(); i++){
        if(i>=minx && i<=maxx2){
            v1.push_back(pair<double,double>(log(i), log(needed[i])));
            v2.push_back(pair<double,double>((double)i, log(needed[i])));   
        }
    }
    
    polreg = regresion(v1);
    expreg = regresion(v2);
    
    t_fin = clock();
    secsDim = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    if(verbose){
        cerr << "dimension = " << -polreg.first << endl;
        cerr << "decay = " << -expreg.first << endl;
    }
    
	//**************************************************************************
    // Computing SELF-SIMILAR Structure (CVIG)
    //**************************************************************************
    
	if(verbose){
        cerr << "Computing SELF-SIMILAR Structure (CVIG)" << endl;
        cerr << "max_clauses: " << max_clauses << endl;
    }

    v1.clear(); v2.clear();
    
    t_ini = clock();        
    needed = computeNeeded(cvig);

    for(int i=1; i<needed.size(); i++){
        if(i>=minx && i<=maxx2){
            v1.push_back(pair<double,double>(log(i), log(needed[i])));
            v2.push_back(pair<double,double>((double)i, log(needed[i])));   
        }
    }
    
    polregB = regresion(v1);
    expregB = regresion(v2);
    
    t_fin = clock();
    secsDib = (double)(t_fin - t_ini) / CLOCKS_PER_SEC; 
    
    if(verbose){
        cerr << "dimension bipartite = " << -polregB.first << endl;
        cerr << "decay bipartite = " << -expregB.first << endl;
    }
  
    //**************************************************************************
    // Modularity VIG
    //**************************************************************************
    
    if(verbose) {
        cerr << "Computing COMMUNITY Structure (VIG)" << endl;
        cerr << "max_clauses: " << max_clauses << endl;
    }


    // Compute
    t_ini = clock();
    modularity = c.compute_modularity_GFA(precision);
    c.compute_communities();
    t_fin = clock();
    secsMod = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    if(verbose){
        cerr << "modularity = " << modularity << endl;
        cerr << "communities = " << (int)c.ncomm << endl;
        cerr << "largest size = " << (double)c.Comm[c.Comm_order[0].first].size()/vig->size() << endl;
        cerr << "iterations = " << c.iterations << endl;
    }
    

    //**************************************************************************    
    // Modularity CVIG
    //**************************************************************************

    
    // Compute
    t_ini = clock();    
    modularity_bip = c_bip.compute_modularity_GFA(precision);
    c_bip.compute_communities();
    t_fin = clock();

    if(verbose){
        cerr << "modularity = " << modularity_bip << endl;
        cerr << "communities = " << (int)c_bip.ncomm << endl;
        cerr << "largest size = " << (double)c_bip.Comm[c_bip.Comm_order[0].first].size()/cvig->size() << endl;
        cerr << "iterations = " << c_bip.iterations << endl;
    }

    secsModBip = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;

    //**************************************************************************
    // End
    //**************************************************************************
    
    secsTotal = secsGraphs + secsAlphaVar + secsAlphaClau + secsDim + secsDib + secsMod + secsModBip;
    
    char * stat_names = "#instances, time-buildGraphs, alphaVarExp, "
                        "time-AlphaVar, alphaClauExp, time-AlphaClau, "
                        "dim, time-dim, dim-bip, "
                        "time-dimBip, mod, #comm-vig, "
                        "time-mod, mod-bip, #comm-cvig, "
                        "time-mod-bip, time-total";

    cerr << "Runtime = " << secsTotal << " secs" << endl;
    cerr << "***************************************" << endl;
    cout << stat_names << endl;
	cout << fin << "," << secsGraphs << "," << alphavarexp << "," << secsAlphaVar << "," << alphaclauexp << "," << secsAlphaClau << "," << -polreg.first << "," << secsDim << "," << -polregB.first << "," << secsDib << "," << modularity << "," << (int)c.ncomm << "," << secsMod << "," << modularity_bip << "," << (int)c_bip.ncomm << "," << secsModBip << ","  << secsTotal << endl;

    return modularity;

}


// C Extension Info Section

static PyObject* FeatSatError;


// Compute All
static PyObject* featsat_compute_all(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }
    
    double ans = compute_all(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}


// Modularity Interfaces
static PyObject* featsat_modularity_vig(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }

   	double ans = modularity_vig(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}

static PyObject* featsat_modularity_cvig(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }
    
    double ans = modularity_cvig(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}


// Scale Free Interfaces
static PyObject* featsat_scale_free_var(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }
    
    double ans = scale_free_var(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}

static PyObject* featsat_scale_free_clause(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }
    
    double ans = scale_free_clause(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}

// Self Similar Interfaces
static PyObject* featsat_self_similar_vig(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }
    
    double ans = self_similar_vig(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}

static PyObject* featsat_self_similar_cvig(PyObject* self, PyObject* args) {

    char* file_name;
    int max_clauses;

    if (!PyArg_ParseTuple(args, "si", &file_name, &max_clauses)) {
        return NULL;
    }
    
    double ans = self_similar_cvig(file_name, max_clauses);
    return Py_BuildValue("d", ans);
}

// Function packaging
static PyMethodDef FeatSatMethods[] = {
    {
        "compute_all",
        featsat_compute_all,
        METH_VARARGS,
        "Computes all available features.\n\n",
    },
    {
        "modularity_vig",
        featsat_modularity_vig,
        METH_VARARGS,
        "Computes Modularity of a given CNF file for VIG representation.\n\n",
    },
    {
        "modularity_cvig",
        featsat_modularity_cvig,
        METH_VARARGS,
        "Computes Modularity of a given CNF file for CVIG representation.\n\n",
    },
    {
        "scale_free_var",
        featsat_scale_free_var,
        METH_VARARGS,
        "Computes Scale Free exponent of a given CNF file in respect to variables.\n\n",
    },
    {
        "scale_free_clause",
        featsat_scale_free_clause,
        METH_VARARGS,
        "Computes Scale Free exponent of a given CNF file in respect to clauses.\n\n",
    },
    {
        "self_similar_vig",
        featsat_self_similar_vig,
        METH_VARARGS,
        "Computes Self Similarity value of a given CNF file for VIG representation.\n\n",
    },
    {
        "self_similar_cvig",
        featsat_self_similar_cvig,
        METH_VARARGS,
        "Computes Self Similarity value of a given CNF file for CVIG representation.\n\n",
    },
    {NULL, NULL, 0, NULL}  // sentinel
};

#if PY_MAJOR_VERSION >= 3
static PyModuleDef featsatmodule = {
    PyModuleDef_HEAD_INIT,
    "featsat",
    "SAT instance analysis module.",
    -1,
    FeatSatMethods,
};

PyMODINIT_FUNC PyInit_featsat() {
    PyObject* module;

    module = PyModule_Create(&featsatmodule);
    if (module == NULL) {
        return NULL;
    }
    FeatSatError = PyErr_NewException("featsat.Error", NULL, NULL);
    Py_INCREF(FeatSatError);
    PyModule_AddObject(module, "Error", FeatSatError);
    return module;
}
#else
PyMODINIT_FUNC initfeatsat() {
    PyObject* module;

    module = Py_InitModule3(
        "featsat", FeatSatMethods, "SAT instance analysis module.");
    if (module == NULL) {
        return;
    }
    ModuError = PyErr_NewException((char*) "featsat.Error", NULL, NULL);
    Py_INCREF(ModuError);
    PyModule_AddObject(module, "Error", FeatSatError);
}
#endif
