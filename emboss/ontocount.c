/** @source ontocount
**
** Get number of ontology term(s) matching a query
**
** @author Copyright (C) 2010 Jon Ison / EMBOSS
** @version 1  First version</replaceable>
** @modified July 2010  Jon Ison First version</replaceable>
** @@
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
********************************************************************/


/* ==================================================================== */
/* ========================== include files =========================== */
/* ==================================================================== */

#include "emboss.h"

/* Inclusion of system and local header files goes here */



/* ==================================================================== */
/* ============================ constants ============================= */
/* ==================================================================== */

/* #define and enum statements go here */



/* ==================================================================== */
/* ======================== global variables ========================== */
/* ==================================================================== */

/* Global variables definitions go here */



/* ==================================================================== */
/* ============================== data ================================ */
/* ==================================================================== */

/* Definition of datatypes go here */



/* ==================================================================== */
/* ==================== function prototypes =========================== */
/* ==================================================================== */

/* Function prototypes for public (external) functions go here */




/* @prog ontocount ************************************************************
**
** Count ontology term(s) matching a query
**
******************************************************************************/

int main(int argc, char **argv)
{
    /* Variable declarations */
    AjPOboall oboall = NULL;
    AjBool subclasses = ajFalse;
    AjBool obsolete = ajFalse;
    AjPFile outf = NULL;

    AjPObo obo     = NULL;
    AjPObo obotest = NULL;

    AjPList obolist = ajListNew();

    AjPTable foundtable = NULL;
    ajuint ifound = 0;
    ajuint ikeep = 0;

    /* ACD processing */
    embInit("ontocount", argc, argv);

    oboall     = ajAcdGetOboall("oboterms");
    outf       = ajAcdGetOutfile("outfile");
    subclasses = ajAcdGetBoolean("subclasses");
    obsolete   = ajAcdGetBoolean("obsolete");
    
    foundtable = ajTablestrNew(600);

    while(ajOboallNext(oboall, &obo))
    {
        ajListPushAppend(obolist, ajOboNewObo(obo));

        if(subclasses)
            ajOboGetTree(obo, obolist);

        while(ajListGetLength(obolist))
        {
            ajListPop(obolist, (void**) &obotest);
            if(!obsolete && ajOboIsObsolete(obotest))
            {
                ajOboDel(&obotest);
                continue;
            }

            ifound++;
            if(!ajTableMatchS(foundtable, obotest->Id))
            {
                ajTablePut(foundtable, ajStrNewS(obotest->Id),
                           (void *) 1);
                ikeep++;
            }
            ajOboDel(&obotest);
        }
    }

    ajFmtPrintF(outf, "%u\n", ikeep);

    /* Memory clean-up and exit */

    ajListFree(&obolist);

    ajOboallDel(&oboall);
    ajOboDel(&obo);
    ajFileClose(&outf);
    ajTablestrFreeKey(&foundtable);
   
    embExit();

    return 0;
}


/* ==================================================================== */
/* ============================ functions ============================= */
/* ==================================================================== */

