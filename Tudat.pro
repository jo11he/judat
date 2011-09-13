# -------------------------------------------------
# Project created by QtCreator 2010-09-30T01:39:33
# -------------------------------------------------
QT -= gui
TARGET = Tudat
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += Applications/exampleEarthOrbitingSatellite.cpp \
    Astrodynamics/Bodies/CelestialBodies/celestialBody.cpp \
    Astrodynamics/Bodies/CelestialBodies/planet.cpp \
    Astrodynamics/Bodies/Vehicles/vehicle.cpp \
    Astrodynamics/Bodies/Vehicles/vehicleExternalModel.cpp \
    Astrodynamics/EnvironmentModels/AtmosphereModel/atmosphereModel.cpp \
    Astrodynamics/EnvironmentModels/AtmosphereModel/exponentialAtmosphere.cpp \
    Astrodynamics/EnvironmentModels/AtmosphereModel/tabulatedAtmosphere.cpp \
    Astrodynamics/EnvironmentModels/AtmosphereModel/unitTestExponentialAtmosphere.cpp \
    Astrodynamics/EnvironmentModels/AtmosphereModel/unitTestTabulatedAtmosphere.cpp \
    Astrodynamics/EnvironmentModels/GravityFieldModel/centralGravityField.cpp \
    Astrodynamics/EnvironmentModels/GravityFieldModel/gravityFieldModel.cpp \
    Astrodynamics/EnvironmentModels/GravityFieldModel/sphericalHarmonicsGravityField.cpp \
    Astrodynamics/EnvironmentModels/GravityFieldModel/unitTestSphericalHarmonicsGravityField.cpp \
    Astrodynamics/ForceModels/forceModel.cpp \
    Astrodynamics/ForceModels/aerodynamicForce.cpp \
    Astrodynamics/ForceModels/gravitationalForceModel.cpp \
    Astrodynamics/ForceModels/unitTestAerodynamicMomentAndAerodynamicForce.cpp \
    Astrodynamics/ForceModels/Aerothermodynamics/aerodynamics.cpp \
    Astrodynamics/ForceModels/Aerothermodynamics/aerodynamicCoefficientGenerator.cpp \
    Astrodynamics/ForceModels/Aerothermodynamics/aerodynamicCoefficientInterface.cpp \
    Astrodynamics/ForceModels/Aerothermodynamics/hypersonicLocalInclinationAnalysis.cpp \
    Astrodynamics/ForceModels/Aerothermodynamics/unitTestAerodynamicsNamespace.cpp \
    Astrodynamics/ForceModels/Aerothermodynamics/unitTestCoefficientGenerator.cpp \
    Astrodynamics/MomentModels/aerodynamicMoment.cpp \
    Astrodynamics/MomentModels/momentModel.cpp \
    Astrodynamics/physicalConstants.cpp \
    Astrodynamics/unitTestPhysicalConstants.cpp \
    Astrodynamics/Propagators/cartesianStateNumericalPropagator.cpp \
    Astrodynamics/Propagators/keplerPropagator.cpp \
    Astrodynamics/Propagators/propagatorDataContainer.cpp \
    Astrodynamics/Propagators/propagator.cpp \
    Astrodynamics/Propagators/seriesPropagator.cpp \
    Astrodynamics/Propagators/numericalPropagator.cpp \
    Astrodynamics/Propagators/unitTestNumericalPropagator.cpp \
    Astrodynamics/Propagators/unitTestKeplerPropagator.cpp \
    Astrodynamics/ReferenceFrames/referenceFrameTransformations.cpp \
    Astrodynamics/ReferenceFrames/unitTestReferenceFrameTransformations.cpp \
    Astrodynamics/States/approximatePlanetPositions.cpp \
    Astrodynamics/States/approximatePlanetPositionsBase.cpp \
    Astrodynamics/States/approximatePlanetPositionsCircularCoplanar.cpp \
    Astrodynamics/States/approximatePlanetPositionsDataContainer.cpp \
    Astrodynamics/States/orbitalElementConversions.cpp \
    Astrodynamics/States/convertMeanAnomalyToEccentricAnomaly.cpp \
    Astrodynamics/States/convertMeanAnomalyToHyperbolicEccentricAnomaly.cpp \
    Astrodynamics/States/unitTestApproximatePlanetPositions.cpp \
    Astrodynamics/States/unitTestKeplerianElements.cpp \
    Astrodynamics/States/unitTestCartesianElements.cpp \
    Astrodynamics/States/unitTestOrbitalElementConversions.cpp \
    Astrodynamics/MissionSegments/trajectoryDesignMethod.cpp \
    Astrodynamics/MissionSegments/DeepSpaceManeuver/unitTestDeepSpaceManeuver.cpp \
    Astrodynamics/MissionSegments/DeepSpaceManeuver/deepSpaceManeuver.cpp \
    Astrodynamics/MissionSegments/EscapeAndCapture/unitTestEscapeAndCapture.cpp \
    Astrodynamics/MissionSegments/EscapeAndCapture/escapePhase.cpp \
    Astrodynamics/MissionSegments/EscapeAndCapture/escapeAndCapture.cpp \
    Astrodynamics/MissionSegments/EscapeAndCapture/capturePhase.cpp \
    Astrodynamics/MissionSegments/GravityAssist/unitTestGravityAssist.cpp \
    Astrodynamics/MissionSegments/GravityAssist/gravityAssist.cpp \
    Astrodynamics/MissionSegments/LambertTargeter/lambertTargeter.cpp \
    Astrodynamics/MissionSegments/LambertTargeter/unitTestLambertTargeter.cpp \
    Astrodynamics/MissionSegments/LibrationPoints/librationPoint.cpp \
    Astrodynamics/MissionSegments/LibrationPoints/unitTestLibrationPoints.cpp \
    Basics/basicFunctions.cpp \
    Input/textFileReader.cpp \
    Input/fileReader.cpp \
    Input/twoLineElementData.cpp \
    Input/twoLineElementsTextFileReader.cpp \
    Input/unitTestTextFileReader.cpp \
    Input/unitTestTwoLineElementsTextFileReader.cpp \
    Mathematics/basicMathematicsFunctions.cpp \
    Mathematics/cubicSplineInterpolation.cpp \
    Mathematics/unitTestBasicMathematicsFunctions.cpp \
    Mathematics/unitTestCubicSplineInterpolation.cpp \
    Mathematics/GeometricShapes/compositeSurfaceGeometry.cpp \
    Mathematics/GeometricShapes/singleSurfaceGeometry.cpp \
    Mathematics/GeometricShapes/torus.cpp \
    Mathematics/GeometricShapes/sphereSegment.cpp \
    Mathematics/GeometricShapes/conicalFrustum.cpp \
    Mathematics/GeometricShapes/capsule.cpp \
    Mathematics/GeometricShapes/unitTestLawgsSurfaceGeometry.cpp \
    Mathematics/GeometricShapes/quadrilateralMeshedSurfaceGeometry.cpp \
    Mathematics/GeometricShapes/lawgsPartGeometry.cpp \
    Mathematics/LinearAlgebra/linearAlgebra.cpp \
    Mathematics/NumericalIntegrators/euler.cpp \
    Mathematics/NumericalIntegrators/integrator.cpp \
    Mathematics/NumericalIntegrators/rungeKutta4thOrderFixedStepsize.cpp \
    Mathematics/NumericalIntegrators/singleStepIntegrationMethods.cpp \
    Mathematics/NumericalIntegrators/unitTestRungeKutta4thOrderFixedStepsizeIntegrator.cpp \
    Mathematics/NumericalIntegrators/unitTestEulerIntegrator.cpp \
    Mathematics/RandomNumberGenerators/exponentialRandomNumberGenerator.cpp \
    Mathematics/RandomNumberGenerators/normalRandomNumberGenerator.cpp \
    Mathematics/RandomNumberGenerators/uniformRandomNumberGenerator.cpp \
    Mathematics/RandomNumberGenerators/unitTestRandomNumberGenerator.cpp \
    Mathematics/RootFindingMethods/unitTestNewtonRaphson.cpp \
    Mathematics/RootFindingMethods/rootFinder.cpp \
    Mathematics/RootFindingMethods/newtonRaphson.cpp \
    Mathematics/Statistics/simpleLinearRegression.cpp \
    Mathematics/Statistics/unitTestSimpleLinearRegression.cpp \
    Mathematics/unitTestUnitConversions.cpp \
    Output/writingOutputToFile.cpp \
    #applicationMain.cpp
    unitTestMain.cpp
HEADERS += Applications/exampleEarthOrbitingSatellite.h \
    Astrodynamics/Bodies/body.h \
    Astrodynamics/Bodies/CelestialBodies/celestialBody.h \
    Astrodynamics/Bodies/CelestialBodies/planet.h \
    Astrodynamics/Bodies/Vehicles/vehicle.h \
    Astrodynamics/Bodies/Vehicles/vehicleExternalModel.h \
    Astrodynamics/EnvironmentModels/environmentModel.h \
    Astrodynamics/EnvironmentModels/AtmosphereModel/atmosphereModel.h \
    Astrodynamics/EnvironmentModels/AtmosphereModel/exponentialAtmosphere.h \
    Astrodynamics/EnvironmentModels/AtmosphereModel/tabulatedAtmosphere.h \
    Astrodynamics/EnvironmentModels/AtmosphereModel/unitTestExponentialAtmosphere.h \
    Astrodynamics/EnvironmentModels/AtmosphereModel/unitTestTabulatedAtmosphere.h \
    Astrodynamics/EnvironmentModels/GravityFieldModel/centralGravityField.h \
    Astrodynamics/EnvironmentModels/GravityFieldModel/gravityFieldModel.h \
    Astrodynamics/EnvironmentModels/GravityFieldModel/sphericalHarmonicsGravityField.h \
    Astrodynamics/EnvironmentModels/GravityFieldModel/unitTestSphericalHarmonicsGravityField.h \
    Astrodynamics/ForceModels/forceModel.h \
    Astrodynamics/ForceModels/aerodynamicForce.h \
    Astrodynamics/ForceModels/gravitationalForceModel.h \
    Astrodynamics/ForceModels/unitTestAerodynamicMomentAndAerodynamicForce.h \
    Astrodynamics/ForceModels/Aerothermodynamics/aerodynamics.h \
    Astrodynamics/ForceModels/Aerothermodynamics/aerodynamicCoefficientGenerator.h \
    Astrodynamics/ForceModels/Aerothermodynamics/aerodynamicCoefficientInterface.h \
    Astrodynamics/ForceModels/Aerothermodynamics/hypersonicLocalInclinationAnalysis.h \
    Astrodynamics/ForceModels/Aerothermodynamics/unitTestAerodynamicsNamespace.h \
    Astrodynamics/ForceModels/Aerothermodynamics/unitTestCoefficientGenerator.h \
    Astrodynamics/MomentModels/aerodynamicMoment.h \
    Astrodynamics/MomentModels/momentModel.h \
    Astrodynamics/physicalConstants.h \
    Astrodynamics/unitTestPhysicalConstants.h \
    Astrodynamics/Propagators/cartesianStateNumericalPropagator.h \
    Astrodynamics/Propagators/keplerPropagator.h \
    Astrodynamics/Propagators/propagatorDataContainer.h \
    Astrodynamics/Propagators/propagator.h \
    Astrodynamics/Propagators/seriesPropagator.h \
    Astrodynamics/Propagators/numericalPropagator.h \
    Astrodynamics/Propagators/unitTestNumericalPropagator.h \
    Astrodynamics/Propagators/unitTestKeplerPropagator.h \
    Astrodynamics/ReferenceFrames/referenceFrameTransformations.h \
    Astrodynamics/ReferenceFrames/unitTestReferenceFrameTransformations.h \
    Astrodynamics/States/approximatePlanetPositions.h \
    Astrodynamics/States/approximatePlanetPositionsBase.h \
    Astrodynamics/States/approximatePlanetPositionsCircularCoplanar.h \
    Astrodynamics/States/approximatePlanetPositionsDataContainer.h \
    Astrodynamics/States/ephemeris.h \
    Astrodynamics/States/state.h \
    Astrodynamics/States/orbitalElements.h \
    Astrodynamics/States/orbitalElementConversions.h \
    Astrodynamics/States/keplerianElements.h \
    Astrodynamics/States/cartesianElements.h \
    Astrodynamics/States/cartesianPositionElements.h \
    Astrodynamics/States/cartesianVelocityElements.h \
    Astrodynamics/States/convertMeanAnomalyBase.h \
    Astrodynamics/States/convertMeanAnomalyToEccentricAnomaly.h \
    Astrodynamics/States/convertMeanAnomalyToHyperbolicEccentricAnomaly.h \
    Astrodynamics/States/unitTestApproximatePlanetPositions.h \
    Astrodynamics/States/unitTestKeplerianElements.h \
    Astrodynamics/States/unitTestCartesianElements.h \
    Astrodynamics/States/unitTestOrbitalElementConversions.h \
    Astrodynamics/MissionSegments/trajectoryDesignMethod.h \
    Astrodynamics/MissionSegments/DeepSpaceManeuver/deepSpaceManeuver.h \
    Astrodynamics/MissionSegments/DeepSpaceManeuver/unitTestDeepSpaceManeuver.h \
    Astrodynamics/MissionSegments/EscapeAndCapture/unitTestEscapeAndCapture.h \
    Astrodynamics/MissionSegments/EscapeAndCapture/escapePhase.h \
    Astrodynamics/MissionSegments/EscapeAndCapture/escapeAndCapture.h \
    Astrodynamics/MissionSegments/EscapeAndCapture/capturePhase.h \
    Astrodynamics/MissionSegments/GravityAssist/unitTestGravityAssist.h \
    Astrodynamics/MissionSegments/GravityAssist/gravityAssist.h \
    Astrodynamics/MissionSegments/LambertTargeter/lambertTargeter.h \
    Astrodynamics/MissionSegments/LambertTargeter/unitTestLambertTargeter.h \
    Astrodynamics/MissionSegments/LibrationPoints/librationPoint.h \
    Astrodynamics/MissionSegments/LibrationPoints/unitTestLibrationPoints.h \
    Basics/basicFunctions.h \
    Input/textFileReader.h \
    Input/fileReader.h \
    Input/twoLineElementData.h \
    Input/twoLineElementsTextFileReader.h \
    Input/unitTestTextFileReader.h \
    Input/unitTestTwoLineElementsTextFileReader.h \
    Mathematics/unitConversions.h \
    Mathematics/unitTestUnitConversions.h \
    Mathematics/basicMathematicsFunctions.h \
    Mathematics/cubicSplineInterpolation.h \
    Mathematics/unitTestBasicMathematicsFunctions.h \
    Mathematics/unitTestCubicSplineInterpolation.h\
    Mathematics/GeometricShapes/compositeSurfaceGeometry.h \
    Mathematics/GeometricShapes/geometricShape.h \
    Mathematics/GeometricShapes/singleSurfaceGeometry.h \
    Mathematics/GeometricShapes/surfaceGeometry.h \
    Mathematics/GeometricShapes/torus.h \
    Mathematics/GeometricShapes/sphereSegment.h \
    Mathematics/GeometricShapes/conicalFrustum.h \
    Mathematics/GeometricShapes/capsule.h \
    Mathematics/GeometricShapes/unitTestLawgsSurfaceGeometry.h \
    Mathematics/GeometricShapes/quadrilateralMeshedSurfaceGeometry.h \
    Mathematics/GeometricShapes/lawgsPartGeometry.h \
    Mathematics/LinearAlgebra/linearAlgebra.h \
    Mathematics/NumericalIntegrators/euler.h \
    Mathematics/NumericalIntegrators/integrator.h \
    Mathematics/NumericalIntegrators/rungeKutta4thOrderFixedStepsize.h \
    Mathematics/NumericalIntegrators/singleStepIntegrationMethods.h \
    Mathematics/NumericalIntegrators/stateDerivativeBase.h \
    Mathematics/NumericalIntegrators/unitTestRungeKutta4thOrderFixedStepsizeIntegrator.h \
    Mathematics/NumericalIntegrators/unitTestEulerIntegrator.h \
    Mathematics/RandomNumberGenerators/exponentialRandomNumberGenerator.h \
    Mathematics/RandomNumberGenerators/normalRandomNumberGenerator.h \
    Mathematics/RandomNumberGenerators/randomNumberGenerator.h \
    Mathematics/RandomNumberGenerators/uniformRandomNumberGenerator.h \
    Mathematics/RandomNumberGenerators/unitTestRandomNumberGenerator.h \
    Mathematics/RootFindingMethods/unitTestNewtonRaphson.h \
    Mathematics/RootFindingMethods/rootFinderBase.h \
    Mathematics/RootFindingMethods/rootFinder.h \
    Mathematics/RootFindingMethods/newtonRaphsonBase.h \
    Mathematics/RootFindingMethods/newtonRaphsonAdaptor.h \
    Mathematics/RootFindingMethods/newtonRaphson.h \
    Mathematics/Statistics/simpleLinearRegression.h \
    Mathematics/Statistics/unitTestSimpleLinearRegression.h \
    Output/outputHandling.h \
    Output/writingOutputToFile.h
INCLUDEPATH += Applications \
    Astrodynamics \
    Astrodynamics/Bodies \
    Astrodynamics/Bodies/CelestialBodies \
    Astrodynamics/Bodies/Vehicles \
    Astrodynamics/EnvironmentModels \
    Astrodynamics/EnvironmentModels/AtmosphereModel \
    Astrodynamics/EnvironmentModels/GravityFieldModel \
    Astrodynamics/ForceModels \
    Astrodynamics/ForceModels/Aerothermodynamics \
    Astrodynamics/MomentModels \
    Astrodynamics/Propagators \
    Astrodynamics/ReferenceFrames \
    Astrodynamics/States \
    Astrodynamics/MissionSegments \
    Astrodynamics/MissionSegments/DeepSpaceManeuver \
    Astrodynamics/MissionSegments/EscapeAndCapture \
    Astrodynamics/MissionSegments/GravityAssist \
    Astrodynamics/MissionSegments/LambertTargeter \
    Astrodynamics/MissionSegments/LibrationPoints \
    Basics \
    External/Eigen-2.0.15 \
    Input \
    Mathematics \
    Mathematics/GeometricShapes \
    Mathematics/LinearAlgebra \
    Mathematics/NumericalIntegrators \
    Mathematics/RandomNumberGenerators \
    Mathematics/RootFindingMethods \
    Mathematics/Statistics \
    Output
