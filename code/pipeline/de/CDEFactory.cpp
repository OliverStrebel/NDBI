//CDEFactory.cpp: Implementierung der Klasse 
//CDEFactory.
//
////////////////////////////////////////////////////////////////////////////////

#include "CDEFactory.h"

#include "pipeline/de/CDE.h"
#include "pipeline/de/CDEMonomeTerm.h"

#include "util/CEADSyConfig.h"
#include "util/CLog.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CDEFactory::CDEFactory()
{
}

CDEFactory::~CDEFactory()
{
}

CDE* CDEFactory::createPolynomialDE(int iNumVariables, int iDegree, int iEquationNum)
{
    std::vector<CDETerm*>* pTermsVector = new std::vector<CDETerm*>();
    if (iDegree < 0) return new CDE(pTermsVector, iEquationNum);

    std::vector<std::vector<int>> powers;
    for (int i = 0; i <= iDegree; i++) powers.push_back(std::vector<int>(1, i)); //Alle Potenzen fuer erste Variable
    for (int k = 1; k < iNumVariables; k++) //Potenzen fuer uebrige Variable
    { 
        std::vector<std::vector<int>> powersCopy = powers; //Auf Kopie arbeiten, um weitere Variable hinzuzufuegen, weil powers waechst
        for (unsigned int m = 0; m < powers.size(); m++) powers[m].push_back(0); //0 kann immer hinzugefuegt werden ohne dass iDegree zu gross wird

        for (int i = 1; i <= iDegree; i++) 
            for(unsigned int j = 0; j < powersCopy.size(); j++)
            {
                int iExponentSum = 0; //Zuerst pruefen, ob durch hinzugefuegte Potenz der Grad nicht ueberschritten wird
                for (int n = 0; n < powersCopy[j].size(); n++) iExponentSum += powersCopy[j][n];
                if (iExponentSum + i <= iDegree) //Falls Grad nicht ueberschritten
                {
                    std::vector<int> monome = powersCopy[j]; //neue Potenz auf Kopie hinzufuegen, weil evtl noch hoehere Potenzen hinzugefuegt werden
                    monome.push_back(i);
                    powers.push_back(monome);
                }
            }
    }

    for (unsigned int i = 0; i < powers.size(); i++) pTermsVector->push_back(new CDEMonomeTerm(powers[i]));
    std::sort(pTermsVector->begin(), pTermsVector->end(), [](CDETerm* p1, CDETerm* p2){return p1->getSortString() < p2->getSortString();});
    return new CDE(pTermsVector, iEquationNum);
}
