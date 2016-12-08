//
//  MovingBarcodeViewController.swift
//  Screen Capture
//
//  Created by Donie Kelly on 05/12/2016.
//  Copyright © 2016 Inhance Technology. All rights reserved.
//

import Foundation

class MovingBarcodeViewController: UIViewController {

    @IBOutlet weak var code1:UIImageView?
    @IBOutlet weak var code2:UIImageView?
    @IBOutlet weak var code3:UIImageView?
    @IBOutlet weak var code4:UIImageView?
    
    override func viewDidLoad() {
        
        let width = self.code1?.frame.size.width
        self.code1?.image = UIImage.mdQRCode(for: "1", size: width!)
        self.code2?.image = UIImage.mdQRCode(for: "2", size: width!)
        self.code3?.image = UIImage.mdQRCode(for: "3", size: width!)
        self.code4?.image = UIImage.mdQRCode(for: "4", size: width!)
    }

    override var prefersStatusBarHidden: Bool {
        return true
    }
}
