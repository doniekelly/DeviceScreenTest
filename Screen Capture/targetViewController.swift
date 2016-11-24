//
//  targetViewController.swift
//  Screen Capture
//
//  Created by Donie Kelly on 24/11/2016.
//  Copyright © 2016 Inhance Technology. All rights reserved.
//

import UIKit

class targetViewController: UIViewController {

    @IBOutlet weak var qrCode:UIImageView?
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        self.configureBarcode()
        
        ChirpSDK.sharedSDK().setChirpHeardBlock ({chirp, error in
            
            if(chirp != nil) {
                
//                if(chirp?.data == "a7cnn9c0li") {
                    self.performSegue(withIdentifier: "screen1", sender: nil)
//                }
            }
        })
    }

    private func configureBarcode() {
        
        self.qrCode?.backgroundColor = UIColor(white: 1, alpha: 1.0);

        let width:CGFloat? = self.qrCode?.frame.size.width
        let qrImage = UIImage.mdQRCode(for: "1232323223123", size: width!)
        self.qrCode?.image = qrImage
    }
}

