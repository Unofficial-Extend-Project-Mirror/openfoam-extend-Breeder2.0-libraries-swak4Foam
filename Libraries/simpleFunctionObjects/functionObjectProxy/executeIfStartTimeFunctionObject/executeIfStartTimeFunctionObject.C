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
    2011-2013, 2015-2017 Bernhard F.W. Gschaider <bgschaid@hfd-research.com>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#include "executeIfStartTimeFunctionObject.H"
#include "addToRunTimeSelectionTable.H"

#include "polyMesh.H"
#include "IOmanip.H"
#include "swakTime.H"

#ifdef darwin
#include "mach-o/dyld.h"
#endif
#ifdef __linux__
#include <unistd.h>
#endif
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(executeIfStartTimeFunctionObject, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        executeIfStartTimeFunctionObject,
        dictionary
    );

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

executeIfStartTimeFunctionObject::executeIfStartTimeFunctionObject
(
    const word& name,
    const Time& t,
    const dictionary& dict
)
:
    conditionalFunctionObjectListProxy(
        name,
        t,
        dict
    ),
    runIfStartTime_(
        readBool(
            dict.lookup("runIfStartTime")
        )
    ),
    executeOnRestart_(
        dict.lookupOrDefault<bool>("executeOnRestart",false)
    ),
    timeIndexRead_(
        time().timeIndex()
    )
{
    Dbug << " constructing " << name << endl;
#ifdef FOAM_FUNCTIONOBJECT_HAS_SEPARATE_WRITE_METHOD_AND_NO_START
    start();
#endif
    if(!dict.found("executeOnRestart")) {
        WarningIn("executeIfStartTimeFunctionObject::executeIfStartTimeFunctionObject")
            << "No entry 'executeOnRestart' in " << dict.name()
                << ". Assuming 'false'" << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool executeIfStartTimeFunctionObject::condition()
{
    if(
        (time().timeIndex()==0 && !executeOnRestart_)
        ||
        (time().timeIndex()==timeIndexRead_ && executeOnRestart_)
    ) {
        return runIfStartTime_;
    } else {
        return !runIfStartTime_;
    }
}

bool executeIfStartTimeFunctionObject::read(const dictionary& dict)
{
    runIfStartTime_=
        readBool(
            dict.lookup("runIfStartTime")
        );

    return conditionalFunctionObjectListProxy::read(dict);
}

} // namespace Foam

// ************************************************************************* //
