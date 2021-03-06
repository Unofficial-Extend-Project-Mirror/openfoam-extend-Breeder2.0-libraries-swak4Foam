/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
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

Contributors/Copyright:
    2011, 2013, 2016-2017 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>
    2016 Mark Olesen <Mark.Olesen@esi-group.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "volFields.H"
#include "surfaceFields.H"
#include "groovyBCJumpFvPatchFields.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

#ifdef FOAM_MAKE_TEMPLATE_PATCHTYPE_FIELD_USES_PRIMITIVES
makeTemplatePatchTypeField(scalar, groovyBCJump);
#else
makeTemplatePatchTypeField
(
    fvPatchScalarField,
    groovyBCJumpFvPatchScalarField
);
#endif

#ifndef FOAM_JUMP_IS_JUMP_CYCLIC
template<>
void Foam::groovyBCJumpFvPatchField<Foam::scalar>::updateCoeffs()
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::jump() with "
            << jumpExpression_ << endl;
    }

    driver_.clearVariables();

    jump_ = driver_.evaluate<scalar>(this->jumpExpression_);

    fixedJumpFvPatchField<scalar>::updateCoeffs();
}
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
