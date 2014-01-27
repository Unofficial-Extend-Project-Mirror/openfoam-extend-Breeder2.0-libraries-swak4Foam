# Lists the files that are going to be used by other scripts

export SWAKLIBS=(groovyBC \
    groovyStandardBCs \
    swakSourceFields \
    simpleFunctionObjects \
    simpleSwakFunctionObjects \
    simpleLagrangianFunctionObjects \
    simpleSearchableSurfaces \
    swakTopoSources \
    swak4FoamParsers \
    swakLagrangianParser \
    swakPythonIntegration* \
    swak*FunctionPlugin \
    swakFunctionObjects)

if [ "$FOAM_DEV" != "" ]
then
    SWAKLIBS+=(swakFiniteArea)
fi

export SWAKUTILS=(funkySetBoundaryField \
    funkySetFields \
    funkyDoCalc \
    calcNonUniformOffsetsForMapped \
    fieldReport \
    funkyPythonPostproc \
    funkySetLagrangianField \
    replayTransientBC)

if [ "$FOAM_DEV" != "" ]
then
    SWAKUTILS+=(funkySetAreaFields)
fi
