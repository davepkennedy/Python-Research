#include <iostream>
#include <string>
#include <Python/Python.h>

#include "foo.h"
#include "bar.h"

using namespace std;

int execute_code (const char* code, PyObject *glb, PyObject *loc) 
{   
    char *msg;
    PyObject *src;
    PyObject *exc, *val, *trb, *obj, *dum;
    
    src = Py_CompileString (code, "<stdin>", Py_single_input);
    
    if (NULL != src)                         /* compiled just fine - */
    {
        dum = PyEval_EvalCode ((PyCodeObject *)src, glb, loc);
        Py_XDECREF (dum);
        Py_XDECREF (src);
        if (PyErr_Occurred ())
            PyErr_Print ();
        return 1;
    }                                        /* syntax error or E_EOF? */
    else if (PyErr_ExceptionMatches (PyExc_SyntaxError))
    {
        PyErr_Fetch (&exc, &val, &trb);        /* clears exception! */
        
        if (PyArg_ParseTuple (val, "sO", &msg, &obj) &&
            !strcmp (msg, "unexpected EOF while parsing")) /* E_EOF */
        {
            Py_XDECREF (exc);
            Py_XDECREF (val);
            Py_XDECREF (trb);
            return 0;
        }
        else                                   /* some other syntax error */
        {
            PyErr_Restore (exc, val, trb);
            PyErr_Print ();
        }
    }
    else                                     /* some non-syntax error */
    {
        PyErr_Print ();
    }
    return -1;
}

int main (int argc, char * const argv[]) {
    Py_Initialize ();
    
    initFooModule();
    initBarModule();
    
    bool done = false;
    char ps1 [] = ">>>";
    char ps2 [] = "...";
    char* prompt = ps1;
    PyObject *glb = PyEval_GetGlobals();
    PyObject *loc;
    PyObject *builtins = PyEval_GetBuiltins ();
    
    std::cout << Py_GetVersion() << std::endl;
    
    
    loc = PyDict_New ();
    if (!glb) {
        glb = PyDict_New ();
    }
    PyDict_SetItemString (glb, "gstr", PyString_FromString("snagglepants"));
    PyDict_SetItemString (glb, "__builtins__", builtins);
    
    string code;
    while (!done) {
        printf ("%s ", prompt);
        char buffer [100];
        cin.getline(buffer, sizeof(buffer));
        code += string(buffer) + "\n";
        
        if (prompt == ps1 || code.find ("\n\n") != string::npos) {
            if (execute_code (code.c_str(), glb, loc)) {
                code.clear();
                prompt = ps1;
            } else {
                prompt = ps2;
            }
        }
    }
    Py_Finalize();
    return 0;
}
