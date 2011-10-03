//  OF-extend Revision: $Id$ 
/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "solveTransportPDE.H"

#include "polyMesh.H"

#include "volFields.H"

#include "FieldValueExpressionDriver.H"

#include "fvScalarMatrix.H"

#include "fvm.H"

namespace Foam {
    defineTypeNameAndDebug(solveTransportPDE,0);
}

Foam::solveTransportPDE::solveTransportPDE
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    solvePDECommon(
        name,
        obr,
        dict,
        loadFromFiles
    ),
    rhoDimension_(dimless),
    diffusionDimension_(dimless),
    sourceDimension_(dimless),
    sourceImplicitDimension_(dimless)
{
    if (!isA<polyMesh>(obr))
    {
        active_=false;
        WarningIn("solveTransportPDE::solveTransportPDE")
            << "Not a polyMesh. Nothing I can do"
                << endl;
    }

    read(dict);

    if(solveAt_==saStartup) {
        solve();
    }
}

Foam::solveTransportPDE::~solveTransportPDE()
{}

void Foam::solveTransportPDE::read(const dictionary& dict)
{
    solvePDECommon::read(dict);

    if(active_) {
        if(!steady_) {
            dict.lookup("rho") >> rhoExpression_ >> rhoDimension_;
        }
        dict.lookup("diffusion") >> diffusionExpression_ >> diffusionDimension_;
        dict.lookup("source") >> sourceExpression_ >> sourceDimension_;
        if(dict.found("sourceImplicit")) {
            dict.lookup("sourceImplicit") 
                >> sourceImplicitExpression_ >> sourceImplicitDimension_;
        }
        dict.lookup("phi") >> phiName_;
    }
}

void Foam::solveTransportPDE::solve()
{
    if(active_) {
        const fvMesh& mesh = refCast<const fvMesh>(obr_);
        dictionary sol=mesh.solutionDict().subDict(fieldName_+"TransportPDE");
        
        FieldValueExpressionDriver &driver=driver_();

        int nCorr=sol.lookupOrDefault<int>("nCorrectors", 0);
        for (int corr=0; corr<=nCorr; corr++) {

            driver.clearVariables();

            driver.parse(diffusionExpression_);
            if(!driver.resultIsScalar()) {
                FatalErrorIn("Foam::solveTransportPDE::solve()")
                    << diffusionExpression_ << " does not evaluate to a scalar"
                        << endl
                        << abort(FatalError);
            }
            volScalarField diffusionField(driver.getScalar());
            diffusionField.dimensions().reset(diffusionDimension_);

            driver.parse(sourceExpression_);
            if(!driver.resultIsScalar()) {
                FatalErrorIn("Foam::solveTransportPDE::solve()")
                    << sourceExpression_ << " does not evaluate to a scalar"
                        << endl
                        << abort(FatalError);
            }
            volScalarField sourceField(driver.getScalar());
            sourceField.dimensions().reset(sourceDimension_);

            volScalarField &f=theField_();
            const surfaceScalarField &phi=mesh.lookupObject<surfaceScalarField>(
                phiName_
            );

            fvMatrix<scalar> eq(
                fvm::div(phi,f)
                -fvm::laplacian(diffusionField,f,"laplacian(diffusion,"+f.name()+")")
                ==
                sourceField
            );

            if(!steady_) {
                driver.parse(rhoExpression_);
                if(!driver.resultIsScalar()) {
                    FatalErrorIn("Foam::solveTransportPDE::solve()")
                        << rhoExpression_ << " does not evaluate to a scalar"
                            << endl
                            << abort(FatalError);
                }
                volScalarField rhoField(driver.getScalar());
                rhoField.dimensions().reset(rhoDimension_);
            
                fvMatrix<scalar> ddtMatrix=fvm::ddt(f);
                if(
                    !ddtMatrix.diagonal()
                    &&
                    !ddtMatrix.symmetric()
                    &&
                    !ddtMatrix.asymmetric()
                ) {
                    // Adding would fail
                } else {
                    eq+=rhoField*ddtMatrix;
                }
            }

            if(sourceImplicitExpression_!="") {
                driver.parse(sourceImplicitExpression_);
                if(!driver.resultIsScalar()) {
                    FatalErrorIn("Foam::solveTransportPDE::solve()")
                        << sourceImplicitExpression_ << " does not evaluate to a scalar"
                            << endl
                            << abort(FatalError);
                }
                volScalarField sourceImplicitField(driver.getScalar());
                sourceImplicitField.dimensions().reset(sourceImplicitDimension_);
            
                eq-=fvm::SuSp(sourceImplicitField,f);
            }

            int nNonOrthCorr=sol.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);
            for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++)
            {
                eq.solve();
            }
        }
    }
}

// ************************************************************************* //