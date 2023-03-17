/*    Copyright (c) 2010-2023, Delft University of Technology
 *    All rights reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <limits>
#include <string>

#include <boost/test/unit_test.hpp>

#include "tudat/basics/testMacros.h"
#include "tudat/simulation/estimation.h"
#include "tudat/simulation/estimation_setup.h"

#include "tudat/io/readOdfFile.h"
#include "tudat/simulation/estimation_setup/processOdfFile.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include "tudat/astro/ground_stations/transmittingFrequencies.h"

namespace tudat
{
namespace unit_tests
{

using namespace tudat::spice_interface;
using namespace tudat::ephemerides;
using namespace tudat::simulation_setup;
using namespace tudat;

BOOST_AUTO_TEST_SUITE( test_dsn_odf_observation_models )

BOOST_AUTO_TEST_CASE( testDsnNWayAveragedDopplerModel )
{

    // Official mission trajectory kernels V2 (already includes planetary kernels)
//    spice_interface::loadStandardSpiceKernels(
//            { "/Users/pipas/Documents/messenger-spice/msgr_040803_150430_150430_od431sc_2.bsp" } );

    // Official mission trajectory kernels V0
//    spice_interface::loadStandardSpiceKernels( );
//    spice_interface::loadSpiceKernelInTudat(
//            { "/Users/pipas/Documents/messenger-spice/msgr_040803_150430_150430_od431sc_0.bsp" } );

    // GSFC kernels (more accurate)
//    spice_interface::loadStandardSpiceKernels( );
//    spice_interface::loadSpiceKernelInTudat( "/Users/pipas/Documents/messenger-spice/msgr_110318_110714_recon_gsfc_1.bsp" );
//    spice_interface::loadSpiceKernelInTudat( "/Users/pipas/Documents/messenger-spice/msgr_110716_120430_recon_gsfc_1.bsp" );
//    spice_interface::loadSpiceKernelInTudat( "/Users/pipas/Documents/messenger-spice/msgr_120501_130430_recon_gsfc_1.bsp" );
//    spice_interface::loadSpiceKernelInTudat( "/Users/pipas/Documents/messenger-spice/msgr_130501_140430_recon_gsfc_1.bsp" );
//    spice_interface::loadSpiceKernelInTudat( "/Users/pipas/Documents/messenger-spice/msgr_140501_150429_recon_gsfc_1.bsp" );

    // MRO SSL kernels
//    spice_interface::loadStandardSpiceKernels( );
//    spice_interface::loadSpiceKernelInTudat( "/Users/pipas/Documents/mro-spice/mro_psp43_ssd_mro95a.bsp" );

    // MRO NAV kernerls
    spice_interface::loadStandardSpiceKernels(
            { "/Users/pipas/Documents/mro-spice/de414.bsp",
              "/Users/pipas/Documents/mro-spice/mar063.bsp",
              "/Users/pipas/Documents/mro-spice/mro_psp43.bsp"} );

    // Define bodies to use.
    std::vector< std::string > bodiesToCreate = { "Earth", "Sun", "Mercury", "Venus", "Mars" };

    // Define light-time perturbing bodies
    std::vector< std::string > lightTimePerturbingBodies = bodiesToCreate;

    // Specify initial time
//    double initialEphemerisTime = 234224663.2 - 10.0 * 86400.0; // 2007
//    double finalEphemerisTime = 234349306.2 + 10.0 * 86400.0; // 2007
//    double initialEphemerisTime = 329140800 - 100.0 * 86400.0; // 2010
//    double finalEphemerisTime = 329400000 + 100.0 * 86400.0; // 2010
//    double initialEphemerisTime = 294455561.18548876 - 10.0 * 86400.0; // 2009
//    double finalEphemerisTime = 294489941.185480893 + 10.0 * 86400.0; // 2009
//    double initialEphemerisTime = 406630870.68285096 - 10.0 * 86400.0; // 2011
//    double finalEphemerisTime = 406717262.6828746 + 10.0 * 86400.0; // 2011
    double initialEphemerisTime = 544795200 - 5.0 * 86400.0; // 2017
    double finalEphemerisTime = 544881600 + 5.0 * 86400.0; // 2017
    double ephemerisTimeStep = 300.0;
    double buffer = 10.0 * ephemerisTimeStep;

    // Create bodies settings needed in simulation
    BodyListSettings bodySettings =
            getDefaultBodySettings(
                bodiesToCreate, initialEphemerisTime - buffer, finalEphemerisTime + buffer,
                "SSB", "J2000", ephemerisTimeStep );

//    bodySettings.at( "Earth" )->rotationModelSettings = gcrsToItrsRotationModelSettings(
//            basic_astrodynamics::iau_2006, "J2000" );
    bodySettings.at( "Earth" )->groundStationSettings = getDsnStationSettings( );

    // Create spacecraft
//    std::string spacecraftName = "Messenger";
    std::string spacecraftName = "MRO";
    bodySettings.addSettings( spacecraftName );
    bodySettings.at( spacecraftName )->ephemerisSettings =
            std::make_shared< InterpolatedSpiceEphemerisSettings >(
                    initialEphemerisTime - buffer, finalEphemerisTime + buffer, ephemerisTimeStep, "SSB", "J2000",
                    std::make_shared< interpolators::LagrangeInterpolatorSettings >( 6 ), spacecraftName );

    // Create bodies
    SystemOfBodies bodies = createSystemOfBodies( bodySettings );

//    std::string fileTag = "2007";
//    std::string fileTag = "2009";
//    std::string fileTag = "2011";
//    std::string fileTag = "2017_ssd";
    std::string fileTag = "2017_nav";
    std::shared_ptr< input_output::OdfRawFileContents > rawOdfFileContents =
//            input_output::readOdfFile( "/Users/pipas/Documents/dsn_trk-2-18/odf07155.dat" );
//            input_output::readOdfFile( "/Users/pipas/Documents/messenger-rawdata-odf/mess_rs_10162_163_odf.dat" );
//            input_output::readOdfFile( "/Users/pipas/Documents/messenger-rawdata-odf/mess_rs_09121_121_odf.dat" );
//            input_output::readOdfFile( "/Users/pipas/Documents/messenger-rawdata-odf/mess_rs_11336_2100_odf.dat" );
            input_output::readOdfFile( "/Users/pipas/Documents/mro-rawdata-odf/mromagr2017_097_1335xmmmv1.odf" );

    std::cout << "Start time from 1950 UTC: " << rawOdfFileContents->dataBlocks_.front()->commonDataBlock_->getObservableTime() << std::endl;
    std::cout << "End time from 1950 UTC: " << rawOdfFileContents->dataBlocks_.back()->commonDataBlock_->getObservableTime() << std::endl;

    for ( unsigned int i = 0; i < rawOdfFileContents->dataBlocks_.size(); ++i )
    {
        if ( rawOdfFileContents->dataBlocks_.at( i )->observableSpecificDataBlock_->dataType_ == 12 )
        {
            std::cout << "Start time from 1950 UTC (12): " << rawOdfFileContents->dataBlocks_.at(i)->commonDataBlock_->getObservableTime() << std::endl;
            break;
        }
    }
    for ( unsigned int i = rawOdfFileContents->dataBlocks_.size() - 1; i >= 0 ; --i )
    {
        if ( rawOdfFileContents->dataBlocks_.at( i )->observableSpecificDataBlock_->dataType_ == 12 )
        {
            std::cout << "End time from 1950 UTC (12): " << rawOdfFileContents->dataBlocks_.at(i)->commonDataBlock_->getObservableTime() << std::endl;
            break;
        }
    }


    // Read and process ODF file data
    std::shared_ptr< ProcessedOdfFileContents > processedOdfFileContents =
            std::make_shared< ProcessedOdfFileContents >(
                    rawOdfFileContents,bodies.getBody( "Earth" ), true, spacecraftName );

    std::cout << std::endl << "Spacecraft name: " << processedOdfFileContents->getSpacecraftName( ) << std::endl;
    std::vector< std::string > groundStationsNames = processedOdfFileContents->getGroundStationsNames( );
    std::cout << "Ground stations: ";
    for ( unsigned int i = 0; i < groundStationsNames.size( ); ++i )
    {
        std::cout << groundStationsNames.at( i ) << " ";
    }
    std::cout << std::endl;

    std::cout << std::setprecision( 18 );
    std::cout << "Start time: " << processedOdfFileContents->getStartAndEndTime( bodies ).first << std::endl;
    std::cout << "End time: " << processedOdfFileContents->getStartAndEndTime( bodies ).second << std::endl;

    std::vector< observation_models::ObservableType > observableTypes = processedOdfFileContents->getProcessedObservableTypes( );
    std::cout << "Observable types: ";
    for ( unsigned int i = 0; i < observableTypes.size( ); ++i )
    {
        std::cout << observableTypes.at( i ) << " ";
    }
    std::cout << std::endl;

    // Create observed observation collection
    std::shared_ptr< observation_models::ObservationCollection< double, double > > observedObservationCollection =
            observation_models::createOdfObservedObservationCollection< double, double >(
                    processedOdfFileContents, bodies );

    std::cout << std::endl << "Observation type start and size:" << std::endl;
    std::map< observation_models::ObservableType, std::pair< int, int > > observationTypeStartAndSize =
            observedObservationCollection->getObservationTypeStartAndSize( );
    for ( auto it = observationTypeStartAndSize.begin(); it != observationTypeStartAndSize.end(); ++it )
    {
        std::cout << it->first << " " << std::get<0>(it->second) << " " << std::get<1>(it->second) << std::endl;
    }

//    std::cout << "Observed observables: "<< observedObservationCollection->getObservationVector( ) << std::endl;

    // Create computed observation collection
    std::vector< std::shared_ptr< observation_models::ObservationModelSettings > > observationModelSettingsList;

    std::vector< std::shared_ptr< observation_models::LightTimeCorrectionSettings > > lightTimeCorrectionSettings =
            { std::make_shared< observation_models::FirstOrderRelativisticLightTimeCorrectionSettings >(
                    lightTimePerturbingBodies ) };

    std::map < observation_models::ObservableType, std::vector< observation_models::LinkEnds > > linkEndsPerObservable =
            observedObservationCollection->getLinkEndsPerObservableType( );
    std::shared_ptr< LightTimeConvergenceCriteria > lightTimeConvergenceCriteria =
            std::make_shared< LightTimeConvergenceCriteria >( true );
    for ( auto it = linkEndsPerObservable.begin(); it != linkEndsPerObservable.end(); ++it )
    {
        for ( unsigned int i = 0; i < it->second.size(); ++i )
        {
//            observationModelSettingsList.push_back(
//                    std::make_shared< observation_models::ObservationModelSettings >(
//                            it->first, it->second.at( i ), lightTimeCorrectionSettings, nullptr, nullptr ) );
            if ( it->first == observation_models::dsn_n_way_averaged_doppler )
            {
                observationModelSettingsList.push_back(
                    std::make_shared< observation_models::DsnNWayAveragedDopplerObservationSettings >(
                            it->second.at( i ), lightTimeCorrectionSettings, nullptr,
                            lightTimeConvergenceCriteria ) );
            }
        }
    }

    std::vector< std::shared_ptr< observation_models::ObservationSimulatorBase< double, double > > >
            observationSimulators = observation_models::createObservationSimulators< double, double >(
                    observationModelSettingsList, bodies );


    std::vector< std::shared_ptr< ObservationSimulationSettings< double > > > observationSimulationSettings =
            observation_models::createOdfObservationSimulationSettingsList< double, double >(
                    observedObservationCollection );


    std::shared_ptr< observation_models::ObservationCollection< double, double > >
            simulatedObservationCollection = simulation_setup::simulateObservations< double, double >(
                    observationSimulationSettings, observationSimulators, bodies );

    std::cout << std::endl << "Observation type start and size:" << std::endl;
    observationTypeStartAndSize = simulatedObservationCollection->getObservationTypeStartAndSize( );
    for ( auto it = observationTypeStartAndSize.begin(); it != observationTypeStartAndSize.end(); ++it )
    {
        std::cout << it->first << " " << std::get<0>(it->second) << " " << std::get<1>(it->second) << std::endl;
    }

//    std::cout << simulatedObservationCollection->getObservationVector( ) << std::endl;

    if ( fileTag == "2007" )
    {
        std::vector< std::string > stations = { "DSS-63", "DSS-14", "DSS-43" };

        for ( unsigned int i = 0; i < 3; ++i )
        {
            std::shared_ptr< ground_stations::PiecewiseLinearFrequencyInterpolator > rampInterpolator =
                    std::dynamic_pointer_cast< ground_stations::PiecewiseLinearFrequencyInterpolator >(
                bodies.at("Earth")->getGroundStation( stations.at( i ) )->getTransmittingFrequencyCalculator( ) );

            std::vector< double > startTimes = rampInterpolator->getStartTimes( );
            std::vector< double > endTimes = rampInterpolator->getEndTimes( );
            std::vector< double > rampRates = rampInterpolator->getRampRates( );
            std::vector< double > startFrequency = rampInterpolator->getStartFrequencies( );

            Eigen::MatrixXd rampData ( startTimes.size( ), 4 );
            for ( unsigned int j = 0; j < startTimes.size( ); ++j )
            {
                rampData( j, 0 ) = startTimes.at( j );
                rampData( j, 1 ) = endTimes.at( j );
                rampData( j, 2 ) = rampRates.at( j );
                rampData( j, 3 ) = startFrequency.at( j );
            }

            std::ofstream file("/Users/pipas/tudatpy-testing/rampData_2007" + stations.at( i ) + ".txt");
            file << std::setprecision( 15 ) << rampData;
            file.close();
        }
    }

    Eigen::Matrix< double, Eigen::Dynamic, 1 > simulatedObservations =
            simulatedObservationCollection->getObservationVector( );

    Eigen::MatrixXd observations ( simulatedObservationCollection->getObservationVector( ).size( ), 4 );
    observations.col( 1 ) = simulatedObservationCollection->getObservationVector( );
    observations.col( 3 ) = observedObservationCollection->getObservationVector( );

    for ( unsigned int i = 0; i < simulatedObservationCollection->getObservationVector( ).size( ); ++i )
    {
        observations( i, 0 ) = simulatedObservationCollection->getConcatenatedTimeVector( ).at( i );
        observations( i, 2 ) = observedObservationCollection->getConcatenatedTimeVector( ).at( i );
    }

    std::ofstream file("/Users/pipas/tudatpy-testing/observations_" + fileTag + ".txt");
    file << std::setprecision( 15 ) << observations;
    file.close();

}

BOOST_AUTO_TEST_SUITE_END( )

}

}