/*    Copyright (c) 2010-2023, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 *
 *    References: 820-013, TRK-2-18 Tracking System Interfaces Orbit Data File Interface, Revision E, 2008, JPL/DSN
 */

#ifndef TUDAT_PROCESSODFFILE_H
#define TUDAT_PROCESSODFFILE_H

#include "tudat/basics/utilities.h"
#include "tudat/io/readOdfFile.h"
#include "tudat/astro/observation_models/observableTypes.h"
#include "tudat/astro/earth_orientation/terrestrialTimeScaleConverter.h"
#include "tudat/astro/basic_astro/timeConversions.h"
#include "tudat/astro/basic_astro/physicalConstants.h"
#include "tudat/astro/ground_stations/transmittingFrequencies.h"
#include "tudat/simulation/estimation_setup/observations.h"
#include "tudat/simulation/estimation_setup/observationSimulationSettings.h"
#include "tudat/simulation/environment_setup/body.h"
#include "tudat/math/interpolators/lookupScheme.h"
#include "tudat/math/quadrature/trapezoidQuadrature.h"

namespace tudat
{

namespace observation_models
{

/*! Get the observable type associated with a given ODF data ID.
 *
 * @param odfId ODF data ID
 * @return Observable type
 */
observation_models::ObservableType getObservableTypeForOdfId( const int odfId );

/*! Get the frequency band associated with a given ODF frequency band ID.
 *
 * @param odfId ODF frequency band ID
 * @return Frequency band
 */
observation_models::FrequencyBands getFrequencyBandForOdfId ( const int odfId );

/*! Get the ground station name associated with a given ODF network and station ID.
 *
 * @param networkId ODF network ID
 * @param stationId ODF ground station ID
 * @return Ground station name
 */
std::string getStationNameFromStationId ( const int networkId, const int stationId );

// Base class defining processed ODF data for a single observable and set of link ends.
class ProcessedOdfFileSingleLinkData
{
public:

    /*!
     * Constructor.
     * @param observableType Observable type.
     * @param receivingStation Name of the receiving ground station.
     */
    ProcessedOdfFileSingleLinkData( observation_models::ObservableType observableType,
                                    std::string receivingStation ):
        receivingStation_( receivingStation ),
        observableType_( observableType )
    { }

    void splitSingleLinkDataBase( std::shared_ptr< ProcessedOdfFileSingleLinkData > firstBlockDataSet,
                                  std::shared_ptr< ProcessedOdfFileSingleLinkData > secondBlockDataSet,
                                  const int splitIndex );

    virtual void splitSingleLinkDataDerived( std::shared_ptr< ProcessedOdfFileSingleLinkData > firstBlockDataSet,
                                             std::shared_ptr< ProcessedOdfFileSingleLinkData > secondBlockDataSet,
                                             const int splitIndex ) = 0;

    void splitSingleLinkData( std::shared_ptr< ProcessedOdfFileSingleLinkData > firstBlockDataSet,
                              std::shared_ptr< ProcessedOdfFileSingleLinkData > secondBlockDataSet,
                              const int splitIndex )
    {
        splitSingleLinkDataDerived( firstBlockDataSet, secondBlockDataSet, splitIndex );
        splitSingleLinkDataDerived( firstBlockDataSet, secondBlockDataSet, splitIndex );
    }

    // Destructor
    virtual ~ProcessedOdfFileSingleLinkData( ){ }

    // Observation times as seconds since EME1950 UTC
    std::vector< double > unprocessedObservationTimes_;
    // Observation times as seconds since J2000 TDB
    std::vector< double > processedObservationTimes_;

    // Value of the observables
    std::vector< Eigen::Matrix< double, Eigen::Dynamic, 1 > > observableValues_;
    // Vector with the downlink delay at the receiver for each observation
    std::vector< double > receiverDownlinkDelays_;

    // Vector with the downlink band ID for each observation
    std::vector< int > downlinkBandIds_;
    // Vector with the uplink band ID for each observation
    std::vector< int > uplinkBandIds_;
    // Vector with the frequency band ID of the reference frequency for each observation
    std::vector< int > referenceBandIds_;

    // Vector of files from which the data for the current observable and link ends was extracted
    std::vector< std::string > originFiles_;

    // Name of the receiving ground station
    std::string receivingStation_;

    // Returns the observables mapped by the observation time
    std::map< double, Eigen::Matrix< double, Eigen::Dynamic, 1 > > getObservables( )
    {
        return utilities::createMapFromVectors( processedObservationTimes_, observableValues_ );
    }

    // Returns a vector with the observables
    std::vector< Eigen::Matrix< double, Eigen::Dynamic, 1 > > getObservablesVector( )
    {
        return observableValues_;
    }

    // Returns a vector with the processed observation times
    std::vector< double > getObservationTimesVector( )
    {
        return processedObservationTimes_;
    }


    std::pair< double, double > getTimeBounds( )
    {
        return std::make_pair ( *std::min_element( processedObservationTimes_.begin( ), processedObservationTimes_.end( ) ),
                                *std::max_element( processedObservationTimes_.begin( ), processedObservationTimes_.end( ) ) );
    }

    // Returns the observable type
    observation_models::ObservableType getObservableType( )
    {
        return observableType_;
    }

protected:

    observation_models::ObservableType observableType_;

};

// Derived class defining Doppler (both 1- and n-way) data for a single set of link ends.
class ProcessedOdfFileDopplerData: public ProcessedOdfFileSingleLinkData
{
public:

    /*!
     * Constructor.
     *
     * @param observableType Observable type.
     * @param receivingStation Name of the receiving ground station.
     * @param transmittingStation Name of the transmitting ground station.
     */
    ProcessedOdfFileDopplerData( observation_models::ObservableType observableType,
                                 std::string receivingStation,
                                 std::string transmittingStation ):
        ProcessedOdfFileSingleLinkData( observableType, receivingStation ),
        transmittingStation_( transmittingStation )
    { }

    // Destructor
    ~ProcessedOdfFileDopplerData( ){ }

    void splitSingleLinkDataDerived( std::shared_ptr< ProcessedOdfFileSingleLinkData > firstBlockDataSet,
                                     std::shared_ptr< ProcessedOdfFileSingleLinkData > secondBlockDataSet,
                                     const int splitIndex );


    // Name of the transmitting ground station
    std::string transmittingStation_;

    // Receiver channel per observation
    std::vector< int > receiverChannels_;
    // Reference frequency per observation
    std::vector< double > referenceFrequencies_;
    // Count interval per observation
    std::vector< double > countInterval_;
    // Uplink delay at the transmitting station per observation
    std::vector< double > transmitterUplinkDelays_;
    // Ramping flag indicating whether ramps should be used to replace receiver reference frequency (if flag is false).
    std::vector< bool > receiverRampingFlags_;

    std::map< double, bool > getReceiverRampingFlags( )
    {
        return utilities::createMapFromVectors( processedObservationTimes_, receiverRampingFlags_ );
    }

    std::map< double, double > getReferenceFrequencies( )
    {
        return utilities::createMapFromVectors( processedObservationTimes_, referenceFrequencies_ );
    }

    std::vector< double > getReferenceFrequenciesVector( )
    {
        return referenceFrequencies_;
    }

    std::map< double, double > getCountInterval( )
    {
        return utilities::createMapFromVectors( processedObservationTimes_, countInterval_ );
    }
};

/*!
 * Compares two raw ODF data objects based on their start date. Used to sort ODF files.
 *
 * @param rawOdfData1 Raw ODF data object.
 * @param rawOdfData2 Raw ODF data object.
 * @return true if rawOdfData1 starts before rawOdfData2, false otherwise
 */
bool compareRawOdfDataByStartDate( std::shared_ptr< input_output::OdfRawFileContents > rawOdfData1,
                                   std::shared_ptr< input_output::OdfRawFileContents > rawOdfData2 );

// Class containing processed ODF data.
class ProcessedOdfFileContents
{
public:

    /*!
     * Constructor for single raw ODF data object. Processes the raw ODF data.
     *
     * @param rawOdfDataVector Vector of multiple ODF data objects
     * @param spacecraftName Name of the spacecraft.
     * @param verbose Bool indicating whether to print warning regarding e.g. ignored data.
     * @param earthFixedGroundStationPositions Map with the position of each ground station in the corresponding planet's
     *      body-fixed frame. Positions are only used for converting the time between UTC and TDB, therefore approximate
     *      positions are sufficient.
     */
    ProcessedOdfFileContents(
            const std::shared_ptr< input_output::OdfRawFileContents > rawOdfData,
            const std::string spacecraftName,
            bool verbose = true,
            const std::map< std::string, Eigen::Vector3d >& earthFixedGroundStationPositions =
                    simulation_setup::getApproximateDsnGroundStationPositions( ) ):
        ProcessedOdfFileContents(
                std::vector< std::shared_ptr< input_output::OdfRawFileContents > >{ rawOdfData },
                spacecraftName, verbose, earthFixedGroundStationPositions )
    { }

    /*!
     * Constructor for multiple ODF data objects. Processes the raw ODF data.
     *
     * @param rawOdfDataVector Vector of multiple ODF data objects
     * @param spacecraftName Name of the spacecraft.
     * @param verbose Bool indicating whether to print warning regarding e.g. ignored data.
     * @param earthFixedGroundStationPositions Map with the position of each ground station in the corresponding planet's
     *      body-fixed frame. Positions are only used for converting the time between UTC and TDB, therefore approximate
     *      positions are sufficient.
     */
    ProcessedOdfFileContents(
            std::vector< std::shared_ptr< input_output::OdfRawFileContents > > rawOdfDataVector,
            const std::string spacecraftName,
            const bool verbose = true,
            const std::map< std::string, Eigen::Vector3d >& earthFixedGroundStationPositions =
                    simulation_setup::getApproximateDsnGroundStationPositions( ) ):
            rawOdfData_( rawOdfDataVector ),
            spacecraftName_( spacecraftName ),
            approximateEarthFixedGroundStationPositions_ ( earthFixedGroundStationPositions ),
            verbose_( verbose )
    {
//        if ( spacecraftName != "" )
//        {
//            spacecraftName_ = spacecraftName;
//        }
//        else
//        {
//            // Spacecraft name selected to be "NAIF Id", which is equal to -"JPL Id" (for a spacecraft)
//            spacecraftName_ = std::to_string( - static_cast< int >( rawOdfDataVector.front( )->spacecraftId_ ) );
//        }

        // Sort ODF data files by date and check whether all the provided files apply to the same spacecraft
        sortAndValidateOdfDataVector( rawOdfDataVector );

        // Extract and process raw ODF data
        extractMultipleRawOdfRampData( rawOdfDataVector );
        for ( unsigned int i = 0; i < rawOdfDataVector.size( ); ++i )
        {
            extractRawOdfOrbitData( rawOdfDataVector.at( i ) );
        }
        // Compute the processed observation times (i.e. TDB time from J2000)
        updateProcessedObservationTimes( );
    }

    // Get the name of the spacecraft to which the ODF data applies
    std::string getSpacecraftName( )
    {
        return spacecraftName_;
    }

    // Get the names of the ground stations included in the ODF files
    std::vector< std::string > getGroundStationsNames( );

    // Get the observable types in the ODF files
    std::vector< observation_models::ObservableType > getProcessedObservableTypes( );

    // Get pair of < start time, end time > of the data contained in the ODF files
    std::pair< double, double > getStartAndEndTime( );

    // Return ODF observable types IDs (as per TRK-2-18) that were not included in the processed data
    std::vector< int > getIgnoredRawOdfObservableTypes( )
    {
        return ignoredRawOdfObservableTypes_;
    }

    // Return ground stations for which observations were not included in the processed data (due to absence of ramp tables)
    std::vector< std::string > getIgnoredGroundStations( )
    {
        return ignoredGroundStations_;
    }

    // Return the ramp interpolators per ground station
    const std::map< std::string, std::shared_ptr< ground_stations::PiecewiseLinearFrequencyInterpolator > >& getRampInterpolators( )
    {
        return rampInterpolators_;
    }

    // Return processed data
    const std::map< observation_models::ObservableType, std::map< observation_models::LinkEnds,
        std::shared_ptr< ProcessedOdfFileSingleLinkData > > >& getProcessedDataBlocks( )
    {
        return processedDataBlocks_;
    }

    // Return the raw ODF data
    std::vector< std::shared_ptr< input_output::OdfRawFileContents > > getRawOdfData( )
    {
        return rawOdfData_;
    }

    void defineSpacecraftAntennaId( const std::string& spacecraft, const std::string& antennaName );

    void defineSpacecraftAntennaId( const std::string& spacecraft, const std::string& antennaName,
                                    const std::map< double, double >& timeIntervals );

private:

    /*!
     * Checks whether the vector of ODF data is valid (i.e. all objects apply to the same spacecraft), and if so, sorts the
     * vector by the date of the ODF objets.
     *
     * @param rawOdfDataVector Vector of raw ODF objects.
     */
    void sortAndValidateOdfDataVector( std::vector< std::shared_ptr< input_output::OdfRawFileContents > >& rawOdfDataVector );

    /*!
     * Checks whether a given observation is valid. Checks if the observation time is covered by the available ramp tables,
     * for the relevant ground station(s).
     *
     * @param rawDataBlock Raw ODF data block
     * @param linkEnds Link ends to which the ODF block applies
     * @param currentObservableType Observable type
     * @return Bool indicating whether observation is valid or not
     */
    bool isObservationValid( std::shared_ptr< input_output::OdfDataBlock > rawDataBlock,
                             observation_models::LinkEnds linkEnds,
                             observation_models::ObservableType currentObservableType );

    /*!
     * Extracts data from a raw ODF file, splitting it based on observable type and link ends.
     *
     * @param rawOdfData Raw ODF data object.
     */
    void extractRawOdfOrbitData( std::shared_ptr< input_output::OdfRawFileContents > rawOdfData );

    /*!
     * Add an unprocessed ODF data block to the processed data object associated with the relevant observable type and
     * link ends.
     *
     * @param rawDataBlock Raw ODF data block.
     * @param singleLinkProcessedData Processed data for the link ends to which the raw data block applies.
     * @param rawDataFileName Name of the file from which the raw data was extracted.
     */
    void addOdfRawDataBlockToProcessedData(
            const std::shared_ptr< input_output::OdfDataBlock > rawDataBlock,
            const std::shared_ptr< ProcessedOdfFileSingleLinkData > singleLinkProcessedData,
            const std::string rawDataFileName );

    /*!
     * Extracts and merges the ramp data from the provided ODF files, creating one frequency interpolator object per ground
     * station.
     *
     * @param rawOdfDataVector Vector of raw ODF data objects.
     */
    void extractMultipleRawOdfRampData(
            std::vector< std::shared_ptr< input_output::OdfRawFileContents > > rawOdfDataVector );

    /*!
     * Goes over all the extracted ibservations and converts the observation times to TDB from J2000.
     */
    void updateProcessedObservationTimes( );

    /*!
     * Converts UTC times from EME1950 to UTC times from J2000.
     *
     * @param observationTimesUtcFromEME1950 UTC times from EME1950.
     * @return UTC times from J2000
     */
    std::vector< double > computeObservationTimesUtcFromJ2000( std::vector< double > observationTimesUtcFromEME1950 );

    /*!
     * Converts UTC times from EME1950 to TDB times from J2000.
     *
     * @param groundStation Name of the fround station
     * @param observationTimesUtcFromEME1950
     * @return TDB times from J2000
     */
    std::vector< double > computeObservationTimesTdbFromJ2000(
            std::string groundStation, std::vector< double > observationTimesUtcFromEME1950 );

    // Vector of raw ODF data
    std::vector< std::shared_ptr< input_output::OdfRawFileContents > > rawOdfData_;

    // Name of the spacecraft
    const std::string spacecraftName_;

//    const std::string antennaName_;

    // Map containing approximate position of ground stations
    const std::map< std::string, Eigen::Vector3d > approximateEarthFixedGroundStationPositions_;

    // Processed data mapped by observable type and link ends
    std::map< observation_models::ObservableType, std::map< observation_models::LinkEnds,
        std::shared_ptr< ProcessedOdfFileSingleLinkData > > > processedDataBlocks_;

    // Transmitting frequency objects mapped by the ground station names
    std::map< std::string, std::shared_ptr< ground_stations::PiecewiseLinearFrequencyInterpolator > > rampInterpolators_;

    // Unprocessed ramp start and end times. Used for pre-processing observations
    std::map< std::string, std::vector< double > > unprocessedRampStartTimesPerStation_;
    std::map< std::string, std::vector< double > > unprocessedRampEndTimesPerStation_;

    // Ignored data: either because processing of the data type has not been implemented or because the required data to
    // simulate the corresponding observable is not available in the ODF files (e.g. ramp tables located in some other ODF
    // file that was not provided).
    // Vector keeping the invalid observable types that were found in the raw ODF data
    std::vector< int > ignoredRawOdfObservableTypes_;
    // Vector keeping the invalid ground stations that were found in the raw ODF data
    std::vector< std::string > ignoredGroundStations_;
    // Vector keeping the invalid data blocks that were found in the raw ODF data
    std::vector< std::shared_ptr< input_output::OdfDataBlock > > ignoredOdfRawDataBlocks_;

    // Flag indicating whether to print warnings
    bool verbose_;

    // TODO: friend class used in unit test. Remove after processing of ODF data type 11 (1-way Doppler) is implemented
    friend class ProcessedOdfFileContentsPrivateFunctionTest;
};

// TODO: friend class used in unit test. Remove after processing of ODF data type 11 (1-way Doppler) is implemented
class ProcessedOdfFileContentsPrivateFunctionTest
{
public:

    static double computeObservationTimesTdbFromJ2000(
            std::shared_ptr< ProcessedOdfFileContents > processedOdfFileContents,
            const std::string groundStation,
            const double observationTimeUtcFromEME1950 )
    {
        return processedOdfFileContents->computeObservationTimesTdbFromJ2000(
                groundStation, { observationTimeUtcFromEME1950 } ).front( );
    }

private:

};

inline std::shared_ptr< ProcessedOdfFileContents > processOdfData(
        const std::vector< std::string >& odfFileNames,
        const std::string& spacecraftName,
        const bool verbose = true,
        const std::map< std::string, Eigen::Vector3d >& earthFixedGroundStationPositions =
                simulation_setup::getApproximateDsnGroundStationPositions( ) )
{
    std::vector< std::shared_ptr< input_output::OdfRawFileContents > > rawOdfDataVector;
    for ( std::string odfFile : odfFileNames )
    {
        rawOdfDataVector.push_back( std::make_shared< input_output::OdfRawFileContents >( odfFile ) );
    }

    return std::make_shared< ProcessedOdfFileContents >(
            rawOdfDataVector, spacecraftName, verbose, earthFixedGroundStationPositions );
}

inline std::shared_ptr< ProcessedOdfFileContents > processOdfData(
        const std::string& odfFileName,
        const std::string& spacecraftName,
        const bool verbose = true,
        const std::map< std::string, Eigen::Vector3d >& earthFixedGroundStationPositions =
                simulation_setup::getApproximateDsnGroundStationPositions( ) )
{
    return processOdfData(
            std::vector< std::string >{ odfFileName },
            spacecraftName, verbose, earthFixedGroundStationPositions );
}

inline std::shared_ptr< ProcessedOdfFileContents > processOdfData(
        const std::vector< std::shared_ptr< input_output::OdfRawFileContents > >& odfFiles,
        const std::string& spacecraftName,
        const bool verbose = true,
        const std::map< std::string, Eigen::Vector3d >& earthFixedGroundStationPositions =
                simulation_setup::getApproximateDsnGroundStationPositions( ) )
{
    return std::make_shared< ProcessedOdfFileContents >(
            odfFiles, spacecraftName, verbose, earthFixedGroundStationPositions );
}

inline std::shared_ptr< ProcessedOdfFileContents > processOdfData(
        const std::shared_ptr< input_output::OdfRawFileContents > odfFile,
        const std::string& spacecraftName,
        const bool verbose = true,
        const std::map< std::string, Eigen::Vector3d >& earthFixedGroundStationPositions =
                simulation_setup::getApproximateDsnGroundStationPositions( ) )
{
    return processOdfData(
            std::vector< std::shared_ptr< input_output::OdfRawFileContents > >{ odfFile },
            spacecraftName, verbose, earthFixedGroundStationPositions );
}

/*!
 * Creates the link ends associated with a given ODF observation block.
 *
 * @param dataBlock ODF data block
 * @param spacecraftName Spacecraft name
 * @return Link ends
 */
observation_models::LinkEnds getLinkEndsFromOdfBlock (
        const std::shared_ptr< input_output::OdfDataBlock > dataBlock,
        std::string spacecraftName );

/*!
 * Creates the ancillary settings for the observations indexed by dataIndex in the provided processed ODF data.
 *
 * @param odfDataContents Processed ODF data.
 * @param dataIndex Index of the observation for which to create the ancillary settings.
 * @return Ancillary settings
 */
template< typename TimeType = double >
observation_models::ObservationAncilliarySimulationSettings createOdfAncillarySettings(
        std::shared_ptr< ProcessedOdfFileSingleLinkData > odfDataContents,
        unsigned int dataIndex )
{
    if ( dataIndex >= odfDataContents->unprocessedObservationTimes_.size( ) )
    {
        throw std::runtime_error("Error when creating ODF data ancillary settings: specified data index is larger than data size.");
    }

    observation_models::ObservationAncilliarySimulationSettings ancillarySettings =
            observation_models::ObservationAncilliarySimulationSettings( );

    observation_models::ObservableType currentObservableType = odfDataContents->getObservableType( );

    // Set common ancillary settings
    std::vector< observation_models::FrequencyBands > frequencyBands = {
            getFrequencyBandForOdfId( odfDataContents->uplinkBandIds_.at( dataIndex ) ),
            getFrequencyBandForOdfId( odfDataContents->downlinkBandIds_.at( dataIndex ) ) };
    ancillarySettings.setAncilliaryDoubleVectorData(
            observation_models::frequency_bands, convertFrequencyBandsToDoubleVector( frequencyBands ) );

    ancillarySettings.setAncilliaryDoubleData(
            observation_models::reception_reference_frequency_band,
            convertFrequencyBandToDouble( getFrequencyBandForOdfId( odfDataContents->referenceBandIds_.at( dataIndex ) ) ) );

    if ( std::dynamic_pointer_cast< ProcessedOdfFileDopplerData >( odfDataContents ) != nullptr )
    {
        std::shared_ptr< ProcessedOdfFileDopplerData > dopplerDataBlock
                = std::dynamic_pointer_cast< ProcessedOdfFileDopplerData >( odfDataContents );

        ancillarySettings.setAncilliaryDoubleData(
                observation_models::doppler_integration_time, dopplerDataBlock->countInterval_.at( dataIndex ) );
        ancillarySettings.setAncilliaryDoubleData(
                 observation_models::doppler_reference_frequency, dopplerDataBlock->referenceFrequencies_.at( dataIndex ) );

        if ( currentObservableType == observation_models::dsn_n_way_averaged_doppler )
        {
            ancillarySettings.setAncilliaryDoubleVectorData(
                    observation_models::link_ends_delays, std::vector< double >{
                    dopplerDataBlock->transmitterUplinkDelays_.at( dataIndex ), 0.0,
                    dopplerDataBlock->receiverDownlinkDelays_.at( dataIndex ) } );
        }
        else
        {
            ancillarySettings.setAncilliaryDoubleVectorData(
                    observation_models::link_ends_delays, std::vector< double >{
                    dopplerDataBlock->transmitterUplinkDelays_.at( dataIndex ),
                    dopplerDataBlock->receiverDownlinkDelays_.at( dataIndex ) } );
        }

    }
    else
    {
        throw std::runtime_error("Error when casting ODF processed data: data type not identified.");
    }

    return ancillarySettings;
}

/*!
 * Given the processed ODF data for a single set of link ends, extracts for each observation the observation time,
 * observable value, and ancillary settings.
 *
 * @param currentObservableType Observable type of data (input)
 * @param odfSingleLinkData Observations data for a single set of link ends (input)
 * @param observationTimes Vector of observation times (output)
 * @param observables Vector of observables (output)
 * @param ancillarySettings Vector of ancillary settings (output)
 */
template< typename ObservationScalarType = double, typename TimeType = double >
void separateSingleLinkOdfData(
        observation_models::ObservableType currentObservableType,
        std::shared_ptr< ProcessedOdfFileSingleLinkData > odfSingleLinkData,
        std::vector< std::vector< TimeType > >& observationTimes,
        std::vector< std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > > >& observables,
        std::vector< observation_models::ObservationAncilliarySimulationSettings >& ancillarySettings )
{
    // Initialize vectors
    observationTimes.clear( );
    observables.clear( );
    ancillarySettings.clear( );

    // Get time and observables vectors
    std::vector< double > observationTimesTdb = odfSingleLinkData->getObservationTimesVector( );
    std::vector< Eigen::Matrix< double, Eigen::Dynamic, 1 > > observablesVector =
            odfSingleLinkData->getObservablesVector( );

    for ( unsigned int i = 0; i < odfSingleLinkData->unprocessedObservationTimes_.size( ); ++i )
    {
        observation_models::ObservationAncilliarySimulationSettings currentAncillarySettings =
                createOdfAncillarySettings< TimeType >( odfSingleLinkData, i );

        bool newAncillarySettings = true;

        for ( unsigned int j = 0; j < ancillarySettings.size( ); ++j )
        {
            if ( ancillarySettings.at( j ) == currentAncillarySettings )
            {
                newAncillarySettings = false;
                observationTimes.at( j ).push_back( static_cast< TimeType >( observationTimesTdb.at( i ) ) );
                observables.at( j ).push_back( observablesVector.at( i ).template cast< ObservationScalarType >( ) );
                break;
            }
        }

        if ( newAncillarySettings )
        {
            observationTimes.push_back ( std::vector< TimeType >{ static_cast< TimeType >( observationTimesTdb.at( i ) ) } );
            observables.push_back( std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > >{
                observablesVector.at( i ).template cast< ObservationScalarType >( ) } );
            ancillarySettings.push_back( currentAncillarySettings );
        }
    }
}

/*!
 * Creates an observation collection containing the provided ODF data. Only the specified observable types are loaded
 * from the processed ODF data into the observation collection.
 *
 * @param processedOdfFileContents Processed ODF data.
 * @param observableTypesToProcess Observable types to process.
 * @return Observation collection.
 */
template< typename ObservationScalarType = double, typename TimeType = double >
std::shared_ptr< observation_models::ObservationCollection< ObservationScalarType, TimeType > > createOdfObservedObservationCollection(
        std::shared_ptr< ProcessedOdfFileContents > processedOdfFileContents,
        std::vector< observation_models::ObservableType > observableTypesToProcess = std::vector< observation_models::ObservableType >( ),
        std::pair< TimeType, TimeType > startAndEndTimesToProcess = std::make_pair< TimeType, TimeType >( TUDAT_NAN, TUDAT_NAN ) )
{
    // Set observables to process
    if ( observableTypesToProcess.empty( ) )
    {
        observableTypesToProcess = processedOdfFileContents->getProcessedObservableTypes( );
    }

    // Create and fill single observation sets
    std::map< observation_models::ObservableType, std::map< observation_models::LinkEnds, std::vector< std::shared_ptr<
            observation_models::SingleObservationSet< ObservationScalarType, TimeType > > > > > sortedObservationSets;

    for ( auto observableTypeIterator = processedOdfFileContents->getProcessedDataBlocks( ).begin( );
            observableTypeIterator != processedOdfFileContents->getProcessedDataBlocks( ).end( ); ++observableTypeIterator )
    {
        observation_models::ObservableType currentObservableType = observableTypeIterator->first;

        // Check if an observation set should be created for the current observable type
        if ( std::count( observableTypesToProcess.begin( ), observableTypesToProcess.end( ), currentObservableType ) == 0 )
        {
            continue;
        }

        for ( auto linkEndsIterator = observableTypeIterator->second.begin( );
                linkEndsIterator != observableTypeIterator->second.end( ); ++linkEndsIterator )
        {
            observation_models::LinkEnds currentLinkEnds = linkEndsIterator->first;
            std::shared_ptr< ProcessedOdfFileSingleLinkData > currentOdfSingleLinkData = linkEndsIterator->second;

            // Get vectors of times, observations, and ancillary settings for the current observable type and link ends
            std::vector< std::vector< TimeType > > observationTimes;
            std::vector< std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > > > observables;
            std::vector< observation_models::ObservationAncilliarySimulationSettings > ancillarySettings;

            // Fill vectors
            separateSingleLinkOdfData(
                    currentObservableType, currentOdfSingleLinkData, observationTimes, observables, ancillarySettings );

            std::vector< std::vector< TimeType > > truncatedObservationTimes;
            std::vector< std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > > > truncatedObservables;

            if ( std::isnan( static_cast< double >( startAndEndTimesToProcess.first ) ) &&
                std::isnan( static_cast< double >( startAndEndTimesToProcess.second ) ) )
            {
                truncatedObservationTimes = observationTimes;
                truncatedObservables = observables;
            }
            else
            {
                for ( unsigned int i = 0; i < observationTimes.size( ); ++i )
                {
                    std::vector< TimeType > singleTruncatedObservationTimes;
                    std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > > singleTruncatedObservables;

                    for ( unsigned int j = 0; j < observationTimes.at( i ).size( ); ++j )
                    {
                        if ( ( (!std::isnan( static_cast< double >( startAndEndTimesToProcess.first ) ) && observationTimes.at( i ).at( j ) >= startAndEndTimesToProcess.first) ||
                            std::isnan( static_cast< double >( startAndEndTimesToProcess.first ) ) ) &&
                            ( (!std::isnan( static_cast< double >( startAndEndTimesToProcess.second ) ) && observationTimes.at( i ).at( j ) <= startAndEndTimesToProcess.second) ||
                            std::isnan( static_cast< double >( startAndEndTimesToProcess.second ) ) ) )
                        {
                            singleTruncatedObservationTimes.push_back( observationTimes.at( i ).at( j ) );
                            singleTruncatedObservables.push_back( observables.at( i ).at( j ) );
                        }
                    }
                    truncatedObservationTimes.push_back( singleTruncatedObservationTimes );
                    truncatedObservables.push_back( singleTruncatedObservables );
                }
            }

            // Create the single observation sets and save them
            for ( unsigned int i = 0; i < observationTimes.size( ); ++i )
            {
                sortedObservationSets[ currentObservableType ][ currentLinkEnds ].push_back(
                    std::make_shared< observation_models::SingleObservationSet< ObservationScalarType, TimeType > >(
                        currentObservableType, currentLinkEnds, truncatedObservables.at( i ), truncatedObservationTimes.at( i ),
                        observation_models::receiver,
                        std::vector< Eigen::VectorXd >( ),
                        nullptr, std::make_shared< observation_models::ObservationAncilliarySimulationSettings >(
                            ancillarySettings.at( i ) ) ) );
            }
        }
    }

    return std::make_shared< observation_models::ObservationCollection< ObservationScalarType, TimeType > >(
            sortedObservationSets );
}

template< typename ObservationScalarType = double, typename TimeType = double >
std::shared_ptr< observation_models::SingleObservationSet< ObservationScalarType, TimeType > > compressDopplerData(
    const std::shared_ptr< observation_models::SingleObservationSet< ObservationScalarType, TimeType > > originalDopplerData,
    const unsigned int compressionRatio )
{

    ObservationScalarType floatingCompressionRatio = mathematical_constants::getFloatingInteger< ObservationScalarType >( compressionRatio );

    double currentCompressionTime = originalDopplerData->getAncilliarySettings( )->getAncilliaryDoubleData( doppler_integration_time );
    double newCompressionTime = static_cast< ObservationScalarType >( compressionRatio ) * currentCompressionTime;

    std::shared_ptr< ObservationAncilliarySimulationSettings > newAncilliarySettings =
        std::make_shared< ObservationAncilliarySimulationSettings >( *(originalDopplerData->getAncilliarySettings( ) ) );
    newAncilliarySettings->setAncilliaryDoubleData( doppler_integration_time, newCompressionTime );

    std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > > originalObservations = originalDopplerData->getObservationsReference( );
    std::vector< TimeType > originalObservationTimes = originalDopplerData->getObservationTimesReference( );

    std::vector< Eigen::Matrix< ObservationScalarType, Eigen::Dynamic, 1 > > compressedObservations;
    std::vector< TimeType > compressedObservationTimes;

    for( unsigned int i = 0; i < originalObservations.size( ); i+= compressionRatio )
    {
        if( originalObservations.size( ) - i > compressionRatio )
        {
            Eigen::Matrix<ObservationScalarType, Eigen::Dynamic, 1> newObservable = originalObservations.at( i );
            TimeType newTime = originalObservationTimes.at( i );

            bool skipObservation = false;
            for ( unsigned int j = 1; ( j < compressionRatio && !skipObservation ); j++ )
            {
                if (( originalObservationTimes.at( i + j ) - originalObservationTimes.at( i + j - 1 ) -
                      currentCompressionTime ) <
                    10.0 * std::numeric_limits<double>::epsilon( ) *
                    static_cast< double >( originalObservationTimes.at( i + j )))
                {
                    newObservable += originalObservations.at( i + j );
                    newTime += originalObservationTimes.at( i + j );
                }
                else
                {
                    skipObservation = true;
                }

            }
            if ( !skipObservation )
            {
                newObservable /= floatingCompressionRatio;
                newTime = newTime / floatingCompressionRatio;

                compressedObservations.push_back( newObservable );
                compressedObservationTimes.push_back( newTime );
            }
        }
    }

    std::shared_ptr< ObservationAncilliarySimulationSettings > ancilliarySimulationSettings =
        std::make_shared< ObservationAncilliarySimulationSettings >( *( originalDopplerData->getAncilliarySettings( ) ) );
    double originalIntegrationTime = ancilliarySimulationSettings->getAncilliaryDoubleData( doppler_integration_time );
    ancilliarySimulationSettings->setAncilliaryDoubleData( doppler_integration_time, originalIntegrationTime * static_cast< double >( floatingCompressionRatio ) );
    return std::make_shared< SingleObservationSet< ObservationScalarType, TimeType > >(
        originalDopplerData->getObservableType( ),
        originalDopplerData->getLinkEnds( ),
        compressedObservations, compressedObservationTimes,
        originalDopplerData->getReferenceLinkEnd( ),
        std::vector< Eigen::VectorXd >( ),
        originalDopplerData->getDependentVariableCalculator( ),
        ancilliarySimulationSettings );
}

template< typename ObservationScalarType = double, typename TimeType = double >
std::shared_ptr< observation_models::ObservationCollection< ObservationScalarType, TimeType > > createCompressedDopplerCollection(
    const std::shared_ptr< observation_models::ObservationCollection< ObservationScalarType, TimeType > > originalDopplerData,
    const unsigned int compressionRatio )
{
    std::map< LinkEnds, std::vector< std::shared_ptr< observation_models::SingleObservationSet< ObservationScalarType, TimeType > > > > uncompressedObservationSets =
        originalDopplerData->getObservations( ).at( dsn_n_way_averaged_doppler );
    std::vector< std::shared_ptr< observation_models::SingleObservationSet< ObservationScalarType, TimeType > > > compressedObservationSets;
    for( auto it : uncompressedObservationSets )
    {
        for( unsigned int i = 0; i < it.second.size( ); i++ )
        {
            std::shared_ptr< observation_models::SingleObservationSet< ObservationScalarType, TimeType > > compressedDataSet =
                compressDopplerData< ObservationScalarType, TimeType >( it.second.at( i ), compressionRatio );
            if( compressedDataSet->getObservationTimes( ).size( ) )
            {
                compressedObservationSets.push_back( compressedDataSet );
            }
        }
    }

    return std::make_shared< observation_models::ObservationCollection< ObservationScalarType, TimeType > >( compressedObservationSets );
}

/*!
 * Function modifies the observable types used in the provided observation simulation settings.
 * It can be used to replace a real observable (extracted from the ODF data, e.g. n-way Doppler) with an idealized
 * observable (e.g. n-way differenced range), since the latter might allows an easier physical interpretation of the
 * problem.
 * This setup only works if the real and idealized observables require the same ancillary settings (or if the ancillary
 * settings required by the latter are a subset of the ones required by the former). If that is not the case, the user
 * should manually modify the observation simulation settigns as needed.
 *
 * @param observationSimulationSettings Observation simulation settings for which the observable types should be modified.
 * @param replacementObservableTypes Map having as keys the observable types to replace and as values the replacement observable types.
 */
template< typename ObservationScalarType = double, typename TimeType = double >
void changeObservableTypesOfObservationSimulationSettings(
        std::vector< std::shared_ptr< simulation_setup::ObservationSimulationSettings< TimeType > > >& observationSimulationSettings,
        const std::map< ObservableType, ObservableType >& replacementObservableTypes =
                { { dsn_n_way_averaged_doppler, n_way_differenced_range },
                  { dsn_one_way_averaged_doppler, one_way_differenced_range } } )
{
    for ( unsigned int i = 0; i < observationSimulationSettings.size( ); ++i )
    {
        ObservableType currentObservableType = observationSimulationSettings.at( i )->getObservableType( );

        if ( replacementObservableTypes.count( currentObservableType ) )
        {
            observationSimulationSettings.at( i )->setObservableType( replacementObservableTypes.at( currentObservableType ) );
        }
    }
}

/*!
 * Sets the transmitting function objects defined via the ODF file in the relevant ground stations.
 *
 * @param processedOdfFileContents Processed ODF file object.
 * @param bodyWithGroundStations Body in which the ground stations are defined
 */
inline void setTransmittingFrequenciesInGroundStations(
        std::shared_ptr< ProcessedOdfFileContents > processedOdfFileContents,
        std::shared_ptr< simulation_setup::Body > bodyWithGroundStations )
{
    for( auto it = processedOdfFileContents->getRampInterpolators( ).begin( );
            it != processedOdfFileContents->getRampInterpolators( ).end( ); it++ )
    {
        if( bodyWithGroundStations->getGroundStationMap( ).count( it->first ) == 0 )
        {
            throw std::runtime_error( "Error when setting frequencies for station " + it->first + ", station not found." );
        }
       bodyWithGroundStations->getGroundStation( it->first )->setTransmittingFrequencyCalculator( it->second );
    }
}

/*!
 * Sets the ODF information required for simulating observations into the system of bodies. This includes:
 *      - Setting the transmitting frequencies objects in the ground stations
 *      - Setting the turnaround ratio in the spacecraft
 *
 * @param processedOdfFileContents Processed ODF file contents.
 * @param bodies System of bodies.
 * @param bodyWithGroundStations Name of the body in which the ground stations are located. Default is "Earth".
 * @param getTurnaroundRatio Function returning the turnaround ratio as a function of the uplink and downlink bands.
 */
void setOdfInformationInBodies(
        const std::shared_ptr< ProcessedOdfFileContents > processedOdfFileContents,
        simulation_setup::SystemOfBodies& bodies,
        const std::string& bodyWithGroundStations = "Earth",
        const std::function< double ( FrequencyBands uplinkBand, FrequencyBands downlinkBand ) > getTurnaroundRatio =
                &getDsnDefaultTurnaroundRatios );

} // namespace observation_models

} // namespace tudat

#endif // TUDAT_PROCESSODFFILE_H
