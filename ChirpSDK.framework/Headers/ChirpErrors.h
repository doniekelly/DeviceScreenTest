/*------------------------------------------------------------------------------
 *
 *  ChirpErrors.h
 *
 *  Errors and exceptions associated with the Chirp SDK.
 *
 *  This file is part of the Chirp SDK for iOS.
 *  For full information on usage and licensing, see http://chirp.io/
 *
 *  Copyright © 2011-2016, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#ifndef ChirpErrors_h
#define ChirpErrors_h

/**
 *  Chirp error states
 */
typedef NS_ENUM(NSInteger, ChirpError)
{
    /**
     *  The application is not active.
     */
    ChirpErrorApplicationDisabled = 10000,
    /**
     *  Missing app key or secret.
     */
    ChirpErrorMissingCredentials = 10001,
    /**
     *  Invalid app key or secret.
     */
    ChirpErrorInvalidCredentials = 10002,
    /**
     *  Insufficient permissions to carry out the requested operation.
     */
    ChirpErrorInsufficientPermissions = 10004,
    /**
     *  Error connecting to the Chirp API server.
     */
    ChirpErrorNetworkError = 21000,
    /**
     *  Object could not be found.
     */
    ChirpErrorEntityNotFound = 20404,
    /**
     *  The specified data could not be serialised as a chirp.
     *  The following classes can be chirped:
     *    NSString, NSNumber, NSDictionary, NSArray
     */
    ChirpErrorGenericError = 30000,
    /**
     *  The parameters are invalid.
     */
    ChirpErrorInvalidParametersForMethod = 30001,
    /**
     *  The device is muted and cannot generate chirp audio.
     */
    ChirpErrorDeviceMuted = 31000,
    /**
     *  The engine is already chirping or streaming.
     */
    ChirpErrorEngineBusy = 31004,
    /**
     *  The shortcode specified was badly formed.
     *  It should be 10 characters drawn from the Chirp alphabet, [0-9a-v].
     */
    ChirpErrorInvalidIdentifier = 32000,
    /**
     *  A chirp was heard but could not be decoded.
     */
    ChirpErrorDecodeFailed = 32006,
    /**
     *  The selected protocol name has not been recognised
     */
    ChirpErrorUnknownProtocolName = 33000,
    /**
     *  Cannot set protocol when engine is running
     */
    ChirpErrorCannotSetProtocolWhenEngineRunning = 33001

};

#endif /* ChirpErrors_h */
