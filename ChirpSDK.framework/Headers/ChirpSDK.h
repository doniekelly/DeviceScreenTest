/*------------------------------------------------------------------------------
 *
 *  ChirpSDK.h
 *
 *  Core SDK functionality.
 *
 *  Full documentation can be found in the `docs` folder of this SDK.
 *
 *  This file is part of the Chirp SDK for iOS.
 *  For full information on usage and licensing, see http://chirp.io/
 *
 *  Copyright © 2011-2016, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#pragma once

@import Accelerate;
@import AudioToolbox;
@import AVFoundation;
@import SystemConfiguration;

#import <ChirpSDK/Chirp.h>
#import <ChirpSDK/ChirpErrors.h>
#import <ChirpSDK/ChirpNotifications.h>

////////////////////////////////////////////////////////////////////////////////
/// @name Constants
////////////////////////////////////////////////////////////////////////////////

/**----------------------------------------------------------------------------*
 *  Chirp audio engine states.
 *----------------------------------------------------------------------------*/
typedef NS_ENUM(NSInteger, ChirpAudioState)
{
    /**
     *  The audio engine is currently stopped
     */
    ChirpAudioStateStopped = 0,
    /**
     *  The audio engine is neither playing nor receiving a chirp
     */
    ChirpAudioStateReady,
    /**
     *  The audio engine is currently playing a chirp
     */
    ChirpAudioStateChirping,
    /**
     *  The audio engine is currently streaming a chirp
     */
    ChirpAudioStateStreaming,
    /**
     *  The audio engine is currently receiving a chirp
     */
    ChirpAudioStateReceiving
};

/**----------------------------------------------------------------------------*
 * When enabled, `streamingMode` instructs the SDK to listen for "streams"
 * of chirps rather than one-shot instances, triggering the `chirpHeardCallback` 
 * as soon as the stream is detected.
 *----------------------------------------------------------------------------*/
typedef NS_ENUM(NSInteger, ChirpStreamingMode)
{
    /**
     *  The SDK's normal mode of operation
     */
    ChirpStreamingModeOff = 0,
    /**
     *  The SDK's streaming mode where chirps are repeated continuously
     */
    ChirpStreamingModeOn
};


/**
 *  Protocols that come packaged with the standard SDK.
 *  Note that non-standard protocols require special app permissions.
 */
#define ChirpProtocolNameStandard     @"standard"
#define ChirpProtocolNameUltrasonic   @"ultrasonic"


/**
 *  The ChirpSDK class encapsulates all of the SDK's major functionality.
 */
@interface ChirpSDK : NSObject

////////////////////////////////////////////////////////////////////////////////
/// @name Creating and configuring an SDK object
////////////////////////////////////////////////////////////////////////////////

/**-----------------------------------------------------------------------------
 *  The shared Chirp SDK instance.
 *
 *  Developers using Swift should use the sharedSDK() method
 *  signature to access the singleton. This overrides the string matching 
 *  behaviour introduced in Swift 3.0 which attempts to force the usage of
 *  ChirpSDK() (object construction) and thus makes the skd() method 
 *  unavailable.
 *
 *  @return Returns a singleton instance of the Chirp SDK.
 *----------------------------------------------------------------------------*/
+ (ChirpSDK * _Nonnull) sdk NS_SWIFT_NAME(sharedSDK());

/**-----------------------------------------------------------------------------
 * Authenticate with the Chirp API server.
 * Receives a callback immediately after authentication has completed.
 *
 * @param key         Your application key from http://developers.chirp.io
 * @param secret      Your secret from http://developers.chirp.io
 * @param completion  An optional completion handler, called after the auth
 *                    server generates a response. Note that this will not
 *                    be triggered when the SDK is used offline (or if the
 *                    app is offline when it starts), so should not be used
 *                    to call `start` or other mission-critical activities.
 *----------------------------------------------------------------------------*/
- (void) setAppKey: (NSString * _Nonnull)key
         andSecret: (NSString * _Nonnull)secret
    withCompletion: (void (^ _Nullable)
                    (BOOL authenticated, NSError * _Nullable error))completion;

/**-----------------------------------------------------------------------------
 * The current version of the SDK.
 *
 * @return Returns a semantic version string.
 *----------------------------------------------------------------------------*/
- (NSString * _Nonnull) version;

/**-----------------------------------------------------------------------------
 * This method is called automatically if playing a chirp is attempted, or if
 * setChirpHeardBlock: is called. It starts the main audio engine running.
 *
 * Accordingly, this method should not need to be called manually unless stop:
 * has been called beforehand.
 *----------------------------------------------------------------------------*/
- (void) start;

/**-----------------------------------------------------------------------------
 * Stop the audio engine running
 *----------------------------------------------------------------------------*/
- (void) stop;

////////////////////////////////////////////////////////////////////////////////
/// @name Receiving chirps
////////////////////////////////////////////////////////////////////////////////

/**-----------------------------------------------------------------------------
 * A block for receiving chirps heard over the air.
 *
 * If not done so already, this method automatically starts the audio engine.
 * i.e. There is no need to call `start` before or as well 
 * as `setChirpHeardBlock`.
 *
 * @param block Block to execute when a chirp is received.
 *
 *              If successful, `chirp` a fully instantiated chirp instance. If
 *              data is associated with this chirp is can then be fetched using
 *              the fetchAssociatedData: Chirp instance method.
 *
 *              If an error occurs, `error` is non-null.
 *----------------------------------------------------------------------------*/
- (void) setChirpHeardBlock:
  (void (^ _Nullable)(Chirp * _Nullable chirp, NSError * _Nullable error))block;

////////////////////////////////////////////////////////////////////////////////
/// @name Audio properties
////////////////////////////////////////////////////////////////////////////////

/**----------------------------------------------------------------------------*
 * I/O block that is triggered when a new buffer of audio is read
 * (typically 256 frames of stereo audio).
 *
 * @param block Callback, passed a mono AudioBuffer containing the new audio.
 *----------------------------------------------------------------------------*/
- (void) setAudioBufferUpdatedBlock:
  ( void (^ _Nullable)(AudioBuffer * _Nonnull buffer, UInt32 numFrames)) block;

/**----------------------------------------------------------------------------*
 * Block that is triggered when the audio engine changes state.
 *
 * @param block Callback, passed the new ChirpAudioState state
 *----------------------------------------------------------------------------*/
- (void) setAudioStateChangedBlock:
(void (^ _Nullable)(ChirpAudioState audioEngineState))block;

/**-----------------------------------------------------------------------------*
 * ChirpAudioState indicates the current activity mode of the audio engine:
 *
 *    ChirpAudioStateStopped: Not running
 *    ChirpAudioStateReady: Inactive, awaiting new chirps
 *    ChirpAudioStateChirping: Active, outputting chirp audio
 *    ChirpAudioStateReceiving: Active, receiving chirp audio
 *
 * @return The current mode of the audio engine
 *----------------------------------------------------------------------------*/
@property (nonatomic, readonly) ChirpAudioState audioEngineState;



/**-----------------------------------------------------------------------------
 * Set and get the Chirp SDK's output volume, independent from the device
 * hardware volume (see below). Defaults to 1.0f.
 *----------------------------------------------------------------------------*/
@property (nonatomic, assign) float volume;

/**-----------------------------------------------------------------------------
 * Returns the hardware audio volume, between 0.0f and 1.0f.
 * This is set by the user using device controls and cannot be modified.
 *
 * This can be checked to generate a warning if the volume is too low.
 *
 * Changes in volume trigger a ChirpNotificationSystemAudioVolumeChanged
 * notification.
 *
 * @return The current system audio setting.
 *----------------------------------------------------------------------------*/
@property (nonatomic, readonly) float systemAudioVolume;

/**-----------------------------------------------------------------------------
 * Returns the current sampling rate of the Chirp SDK.
 * This is usually only needed for diagnostics.
 *
 * @return The current sample rate of the SDK
 *----------------------------------------------------------------------------*/
@property (nonatomic, readonly) float sampleRate;

/**-----------------------------------------------------------------------------
 * Controls the SDK's streaming mode.
 *
 * This is intended for the detection of chirp "streams", in which the same
 * code is played multiple times on repeat.
 *
 * When in streaming mode, a chirp is only reported as being heard once,
 * even if it is heard several times.
 *----------------------------------------------------------------------------*/
@property (nonatomic, assign) ChirpStreamingMode streamingMode;

/**-----------------------------------------------------------------------------
 * Returns YES if a chirp is currently being streamed.
 *----------------------------------------------------------------------------*/
@property (nonatomic, readonly, assign) BOOL isStreaming;

/**-----------------------------------------------------------------------------
 * The last chirp heard by the SDK.
 * Returns `nil` if no chirp has been received.
 *----------------------------------------------------------------------------*/
@property (nullable, nonatomic, readonly) Chirp *lastHeardChirp;


/**-----------------------------------------------------------------------------
 * Select the Chirp audio protocol with the given name.
 *
 * Valid names include:
 *  - ChirpProtocolNameStandard: Audible, 50-bit chirps
 *  - ChirpProtocolNameUltrasonic: Inaudible, 32-bit chirps
 *
 * Note that your application needs to be granted special permission to use
 * non-standard chirps. Please contact <developers@chirp.io> to request
 * access.
 *----------------------------------------------------------------------------*/
- (NSError * _Nullable)setProtocolNamed:(NSString * _Nonnull)protocolName;

////////////////////////////////////////////////////////////////////////////////
/// @name Helper utilities
////////////////////////////////////////////////////////////////////////////////


/**-----------------------------------------------------------------------------
 * Generate a random valid chirp identifier.
 *
 * e.g. 8nk34aa0e0
 *----------------------------------------------------------------------------*/
- (NSString * _Nonnull) randomIdentifier;
- (NSString * _Nonnull) randomShortcode
DEPRECATED_MSG_ATTRIBUTE("Use randomIdentifier: instead");

/**----------------------------------------------------------------------------*
 * Returns YES the given string is able to be chirped directly.
 *
 * @param identifier An NSString instance.
 *
 * @return YES if the identifier can be chirped as-is.
 *----------------------------------------------------------------------------*/
- (BOOL) isValidChirpIdentifier:(NSString * _Nonnull)identifier;
- (BOOL) isValidShortcode:(NSString * _Nonnull)shortcode
DEPRECATED_MSG_ATTRIBUTE("Use isValidChirpIdentifier: instead");

@end
