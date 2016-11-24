/*------------------------------------------------------------------------------
 *
 *  Chirp.h
 *
 *  Represents a single Chirp entity, with attached identifier and data.
 *
 *  This file is part of the Chirp SDK for iOS.
 *  For full information on usage and licensing, see http://chirp.io/
 *
 *  Copyright © 2011-2016, Asio Ltd.
 *  All rights reserved.
 *
 *----------------------------------------------------------------------------*/

#import <Foundation/Foundation.h>
#import "AvailabilityMacros.h"


/**------------------------------------------------------------------------*
 * A single Chirp object.
 *------------------------------------------------------------------------*/
@interface Chirp : NSObject

/**------------------------------------------------------------------------
 * Initialise a `Chirp` object with an identifier. This method depends
 * on the identifier being non-nil and valid for the Chirp SDK's currently
 * set protocol. If the identifier is not valid for the current protocol,
 * the initWithIdentifier: method returns nil.
 *
 * @param identifier a valid identifier
 *
 * @return an instantiated `Chirp` object
 *------------------------------------------------------------------------*/
- (instancetype _Nullable) initWithIdentifier:(NSString * _Nullable) identifier;
- (instancetype _Nullable) initWithShortcode:(NSString * _Nullable) shortcode DEPRECATED_MSG_ATTRIBUTE("Use initWithIdentifier: instead");

/**------------------------------------------------------------------------
 * Initialise a Chirp object with an array of `NSNumber` integer values.
 * This method depends on the identifier being non-nil and valid for the 
 * Chirp SDK's currently set protocol. If the identifier is not valid for 
 * the current protocol, the initWithArray: method returns nil.
 *
 * @param array A valid array of `NSNumber` objects.
 *
 * @return An instantiated `Chirp` object
 *------------------------------------------------------------------------*/
- (instancetype _Nullable) initWithArray:(NSArray * _Nullable) array;

/**------------------------------------------------------------------------
 * The unique identifier of this chirp
 *------------------------------------------------------------------------*/
@property (nullable, nonatomic, strong, readonly) NSString *identifier;
@property (nullable, nonatomic, strong, readonly) NSString *shortcode DEPRECATED_MSG_ATTRIBUTE("Use identifier instead");

/**------------------------------------------------------------------------
 * The identifier + error correction
 *------------------------------------------------------------------------*/
@property (nonatomic, strong, readonly) NSString * _Nullable encodedIdentifier;
@property (nonatomic, strong, readonly) NSString * _Nullable longcode DEPRECATED_MSG_ATTRIBUTE("Use encodedIdentifier instead");

/**------------------------------------------------------------------------
 * The raw array of numeric data that corresponds to the chirp's
 * identifier.
 *------------------------------------------------------------------------*/
@property (nonnull, nonatomic, strong, readonly) NSArray *array;

/**------------------------------------------------------------------------
 * The date this chirp was created
 *------------------------------------------------------------------------*/
@property (nonnull, nonatomic, strong, readonly) NSDate *createdAt;

/**------------------------------------------------------------------------
 * The NSDictionary data associated with this chirp
 *------------------------------------------------------------------------*/
@property (nullable, nonatomic, strong) NSDictionary *data;

/**------------------------------------------------------------------------
 * Play this chirp to the current audio output
 *------------------------------------------------------------------------*/
- (NSError  * _Nullable) chirp;

/**------------------------------------------------------------------------
 * Begin playing this chirp repeatedly to the current audio output. 
 * Must be subsequently followed by a call to stopStreaming to end the stream.
 *------------------------------------------------------------------------*/
- (NSError  * _Nullable) startStreaming;

/**------------------------------------------------------------------------
 * Stop streaming this chirp to the current audio output.
 *------------------------------------------------------------------------*/
- (void) stopStreaming;

/**------------------------------------------------------------------------
 * Play this chirp to the current audio output
 *
 * @param completion An optional completion handler, triggered after
 *                   playback begins.
 *------------------------------------------------------------------------*/
- (void) chirpWithCompletion:(void (^ _Nullable)(Chirp * _Nullable chirp, NSError * _Nullable error)) completion DEPRECATED_MSG_ATTRIBUTE("use chirp: instead")
;

/**------------------------------------------------------------------------
 * Fetch the network data associated with this chirp through the API
 *
 * Completion handler parameters:
 *
 * `chirp` is guaranteed to be non-nil. If the associated data cannot
 * be resolved, chirp.data will be nil, and an error object will be
 * provided describing the issue.
 * 
 * Common errors include problems with network reachability or chirp
 * identifiers that do not have network data associated with them (the
 * equivalent of an HTTP 404 status code).
 *
 * @param completion A required completion handler.
 *------------------------------------------------------------------------*/
- (void) fetchAssociatedDataWithCompletion:(void (^ _Nullable)(Chirp * _Nullable chirp, NSError * _Nullable error)) completion;

/**-----------------------------------------------------------------------------
 * Associate a dictionary of data with a chirp.
 *
 * This uploads the data over the network to the Chirp API and associates it
 * with a unique chirp identifier. A full chirp object is returned with this
 * identifier and its associated data.
 *
 * To immediately chirp the data via the speaker, use `chirp:withCompletion:`
 * or add a `chirp:` call in your completion handler.
 *
 * Receiving devices can then fetch this associated data over the network by
 * calling `fetchAssociatedData:`.
 *
 * Keys must be instances of `NSString`.
 * Values can be instances of `NSString`, `NSNumber`, `NSArray`, `NSDictionary`.
 *
 * @param data Dictionary of content to be chirped.
 * @param completion Completion block, called when the chirp has been created.
 *                   Can be null.
 *
 *                   If successful, `chirp` contains the complete chirp object,
 *                   including its associated data.
 *
 *                   If an error occurs, `error` is non-null.
 *----------------------------------------------------------------------------*/
+ (void) createChirpWithAssociatedData:(NSDictionary * _Nonnull) data
                         andCompletion:(void (^ _Nullable)(Chirp * _Nullable chirp, NSError * _Nullable error)) completion;

/**-----------------------------------------------------------------------------
 * Retrieve a previously-created chirp and its associated data.
 *
 * This queries the API for data associated with the given identifier. If data
 * associated with this identifier is found, a fully instantiated `Chirp` object
 * is returned, with associated data accessible via the instance's `NSDictionary`
 * `data` property.
 *
 * To immediately chirp the data via the speaker, use `chirp:withCompletion:`
 * or add a chirp: call in your completion handler.
 *
 * @param identifier The chirp's `NSString` identifier
 * @param completion Completion block
 *
 *                   If successful, `chirp` contains the complete chirp object,
 *                   including its associated data.
 *
 *                   If an error occurs, `error` is non-null.
 *----------------------------------------------------------------------------*/
+ (void) fetchChirpByIdentifier:(NSString * _Nonnull) identifier
                  andCompletion:(void (^ _Nullable)(Chirp * _Nullable chirp, NSError * _Nullable error)) completion;


@end
