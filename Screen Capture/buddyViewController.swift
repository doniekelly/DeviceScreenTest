//
//  clientViewController.swift
//  Screen Capture
//
//  Created by Donie Kelly on 24/11/2016.
//  Copyright © 2016 Inhance Technology. All rights reserved.
//

import UIKit



class buddyViewController: UIViewController {
    
    var videoFilter: CoreImageVideoFilter?
    var detector: CIDetector?
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        // Create the video filter
        videoFilter = CoreImageVideoFilter(superview: view, applyFilterCallback: nil)
        videoFilter!.startFiltering()
        
        
        detector = prepareQRCodeDetector()
        videoFilter?.applyFilter = {
            image in
            let found = self.performQRCodeDetection(image)
            DispatchQueue.main.async {
                if found.decode != "" {
                    
                    self.videoFilter!.stopFiltering()
                    print(found.decode)
                    
                    let chirpID = ChirpSDK.sharedSDK().randomIdentifier()
                    print(chirpID)
                    let chirp:Chirp? = Chirp(identifier: "719a288e")
                    _ = chirp?.chirp()
                    
                    self.startRectangleDetection()
                }
            }
            return found.outImage
        }
    }

    func startRectangleDetection() {
        
        self.detector = self.prepareRectangleDetector()
        self.videoFilter?.applyFilter = {
            image in
            
            return self.performRectangleDetection(image)
        }
        
        self.videoFilter!.startFiltering()
    }
 
    func performQRCodeDetection(_ image: CIImage) -> (outImage: CIImage?, decode: String) {
        var resultImage: CIImage?
        var decode = ""
        if let detector = detector {
            let features = detector.features(in: image)
            for feature in features as! [CIQRCodeFeature] {
                resultImage = drawHighlightOverlayForPoints(image, topLeft: feature.topLeft, topRight: feature.topRight,
                                                            bottomLeft: feature.bottomLeft, bottomRight: feature.bottomRight)
                decode = feature.messageString!
            }
        }
        return (resultImage, decode)
    }
    
    func performRectangleDetection(_ image: CIImage) -> CIImage? {
        var resultImage: CIImage?
        if let detector = detector {
            // Get the detections
            let features = detector.features(in: image)
            for feature in features as! [CIRectangleFeature] {
                resultImage = drawHighlightOverlayForPoints(image, topLeft: feature.topLeft, topRight: feature.topRight,
                                                            bottomLeft: feature.bottomLeft, bottomRight: feature.bottomRight)
                
                // Get width and height of image
                let imageWidth = image.extent.size.width
                let imageHeight = image.extent.size.height
                let xPosLeft =  imageWidth / 10
                let xPosRight = xPosLeft * 9
                let yPos = imageHeight / 10 // Top 10% 
                
                // Calculate size of tracked object
                let width = feature.bounds.size.width
                let height = feature.bounds.size.height
                
                // top left must be around 10x10 in percentages
                
                
                let area = width * height
                
                NSLog("Area is %.0f. Width = %.0f Height = %.0f", area, width, height)
                if (width > 650 && width < 700 && height > 400 && height < 430)
                {
                    self.videoFilter?.stopFiltering()
                    self.videoFilter!.takePhoto()
                    self.videoFilter?.startFiltering()   
                }
            }
        }
        return resultImage
    }
    
    func prepareQRCodeDetector() -> CIDetector {
        let options = [CIDetectorAccuracy: CIDetectorAccuracyHigh]
        return CIDetector(ofType: CIDetectorTypeQRCode, context: nil, options: options)!
    }
    
    func prepareRectangleDetector() -> CIDetector {
        let options: [String: AnyObject] = [CIDetectorAccuracy: CIDetectorAccuracyHigh as AnyObject, CIDetectorAspectRatio: 1.0 as AnyObject]
        return CIDetector(ofType: CIDetectorTypeRectangle, context: nil, options: options)!
    }
    
    func drawHighlightOverlayForPoints(_ image: CIImage, topLeft: CGPoint, topRight: CGPoint,
                                       bottomLeft: CGPoint, bottomRight: CGPoint) -> CIImage {
        var overlay = CIImage(color: CIColor(red: 0.0, green: 1.0, blue: 0, alpha: 0.6))
        overlay = overlay.cropping(to: image.extent)
        overlay = overlay.applyingFilter("CIPerspectiveTransformWithExtent",
                                         withInputParameters: [
                                            "inputExtent": CIVector(cgRect: image.extent),
                                            "inputTopLeft": CIVector(cgPoint: topLeft),
                                            "inputTopRight": CIVector(cgPoint: topRight),
                                            "inputBottomLeft": CIVector(cgPoint: bottomLeft),
                                            "inputBottomRight": CIVector(cgPoint: bottomRight)
            ])
        return overlay.compositingOverImage(image)
    }
}
