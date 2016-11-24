//
//  clientViewController.swift
//  Screen Capture
//
//  Created by Donie Kelly on 24/11/2016.
//  Copyright © 2016 Inhance Technology. All rights reserved.
//

import UIKit
import CoreImage

class buddyViewController: UIViewController, AVCaptureMetadataOutputObjectsDelegate {
    
    @IBOutlet var qrDecodeLabel: UILabel!
    //    @IBOutlet var detectorModeSelector: UISegmentedControl!
    
    var videoFilter: CoreImageVideoFilter?
    var detector: CIDetector?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        // Create the video filter
        videoFilter = CoreImageVideoFilter(superview: view, applyFilterCallback: nil)
        
        // Simulate a tap on the mode selector to start the process
        //        detectorModeSelector.selectedSegmentIndex = 0
        detectionChange(mode: 1)
    }
    
    func detectionChange(mode:UInt8) {
        
        if let videoFilter = videoFilter {
            videoFilter.stopFiltering()
            self.qrDecodeLabel.isHidden = true
            
            switch mode {
            case 0:
                detector = prepareRectangleDetector()
                videoFilter.applyFilter = {
                    image in
                    return self.performRectangleDetection(image)
                }
            case 1:
                self.qrDecodeLabel.isHidden = false
                detector = prepareQRCodeDetector()
                videoFilter.applyFilter = {
                    image in
                    let found = self.performQRCodeDetection(image)
                    DispatchQueue.main.async {
                        if found.decode != "" {
                            
                            // Switch to rectangle detections
                            self.detectionChange(mode: 0)
                            
                            // Found the QR Code
                            self.qrDecodeLabel.text = found.decode
                            
                            // Chirp out to change to next Screen
                            let chirp:Chirp? = Chirp(identifier: "a7cnn9c0li")
                            _ = chirp?.chirp()
                        }
                    }
                    return found.outImage
                }
            default:
                videoFilter.applyFilter = nil
            }
            
            videoFilter.startFiltering()
        }
    }
    
    
    //MARK: Utility methods
    func performRectangleDetection(_ image: CIImage) -> CIImage? {
        var resultImage: CIImage?
        if let detector = detector {
            // Get the detections
            let features = detector.features(in: image)
            for feature in features as! [CIRectangleFeature] {
                resultImage = drawHighlightOverlayForPoints(image, topLeft: feature.topLeft, topRight: feature.topRight,
                                                            bottomLeft: feature.bottomLeft, bottomRight: feature.bottomRight)
            }
        }
        return resultImage
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
    
    func prepareRectangleDetector() -> CIDetector {
        let options: [String: AnyObject] = [CIDetectorAccuracy: CIDetectorAccuracyHigh as AnyObject, CIDetectorAspectRatio: 1.0 as AnyObject]
        return CIDetector(ofType: CIDetectorTypeRectangle, context: nil, options: options)!
    }
    
    func prepareQRCodeDetector() -> CIDetector {
        let options = [CIDetectorAccuracy: CIDetectorAccuracyHigh]
        return CIDetector(ofType: CIDetectorTypeQRCode, context: nil, options: options)!
    }
    
    func drawHighlightOverlayForPoints(_ image: CIImage, topLeft: CGPoint, topRight: CGPoint,
                                       bottomLeft: CGPoint, bottomRight: CGPoint) -> CIImage {
        var overlay = CIImage(color: CIColor(red: 0.0, green: 1.0, blue: 0.6, alpha: 0.4))
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
