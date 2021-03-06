/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
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

Class
    Foam::fracSmallerThanFromOtherPluginFunction

Description

SourceFiles
    fracSmallerThanFromOtherPluginFunction.C

Contributors/Copyright:
    2016 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#ifndef fracSmallerThanFromOtherPluginFunction_H
#define fracSmallerThanFromOtherPluginFunction_H

#include "FieldValuePluginFunction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class fracSmallerThanFromOtherPluginFunction Declaration
\*---------------------------------------------------------------------------*/
class fracSmallerThanFromOtherPluginFunction
:
    public FieldValuePluginFunction
{
    //- Disallow default bitwise assignment
    void operator=(const fracSmallerThanFromOtherPluginFunction &);

    fracSmallerThanFromOtherPluginFunction(const fracSmallerThanFromOtherPluginFunction &);

    autoPtr<volScalarField> field_;

    autoPtr<volScalarField> other_;

    autoPtr<volScalarField> weight_;

    autoPtr<volScalarField> mask_;

    label binNumber_;

protected:

    void setArgument(
        label index,
        const string &content,
        const CommonValueExpressionDriver &driver
    );

    void setArgument(label index,const label &);

public:
    fracSmallerThanFromOtherPluginFunction(
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    );

    virtual ~fracSmallerThanFromOtherPluginFunction() {}

    TypeName("fracSmallerThanFromOtherPluginFunction");

    void doEvaluation();
};



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
