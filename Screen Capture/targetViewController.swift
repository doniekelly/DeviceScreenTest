//
//  targetViewController.swift
//  Screen Capture
//
//  Created by Donie Kelly on 24/11/2016.
//  Copyright © 2016 Inhance Technology. All rights reserved.
//

import UIKit

class targetViewController: UIViewController {

    private var chirpId:String! = nil
    private var goodScreenId:String! = nil
    private var badScreenId:String! = nil
    
//    @IBOutlet weak var resultLbl: UILabel!
    @IBOutlet weak var qrCode:UIImageView?
    @IBOutlet weak var nextBtn:UIButton?
    @IBOutlet weak var brightness: UISlider!
    @IBOutlet weak var brightnessLabel: UILabel!
    @IBOutlet weak var colorSelect: UISegmentedControl!
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        ChirpSDK.sharedSDK().setAppKey("oea5PoKMIBDLdCS0odHNezUvR", andSecret: "KK4tDFffcNqIwu03aWg47WmGUWrf21iRhzEX3cYQEW96PDTXqR", withCompletion: {authenticated, error in
            
            if(authenticated) {
                print("Chirp Authenticated");
                
            }
            else {
                print(ChirpSDK.sharedSDK().version());
            }
        })
        
        ChirpSDK.sharedSDK().setProtocolNamed(ChirpProtocolNameUltrasonic)
        ChirpSDK.sharedSDK().setChirpHeardBlock ({chirp, error in
            
            if(chirp != nil) {
                
                if(chirp?.identifier == self.chirpId) {
                    self.hideScreenItems(hide:true)
                }
            }
        })
    }
    
    override func viewDidAppear(_ animated: Bool) {
        
        self.configureBarcode()
    }
    
    override var prefersStatusBarHidden: Bool {
        return true
    }

    private func configureBarcode() {
        
        self.qrCode?.backgroundColor = UIColor(white: 1, alpha: 1.0);
        
        self.chirpId = ChirpSDK.sharedSDK().randomIdentifier()
        self.goodScreenId = ChirpSDK.sharedSDK().randomIdentifier()
        self.badScreenId = ChirpSDK.sharedSDK().randomIdentifier()
        
        let  qrText = "131163292639320:" + self.chirpId // + ":" + self.goodScreenId + ":" + self.badScreenId
        
        let width:CGFloat? = self.qrCode?.frame.size.width
        let qrImage = UIImage.mdQRCode(for: qrText, size: width!)
        self.qrCode?.image = qrImage
        
        print(qrText)
    }
    
    @IBAction func touched(_ sender: Any) {
        
        if(self.qrCode?.isHidden)! {
            self.hideScreenItems(hide:false)
        }
    }

    @IBAction func changeBrightness(_ sender: UISlider) {
        
        
        UIScreen.main.brightness = (CGFloat)(sender.value)
    }
    
    @IBAction func ChangeColor(_ sender: UISegmentedControl) {
        
        switch(sender.selectedSegmentIndex)
        {
            case 1:
                self.view.backgroundColor = UIColor.lightGray
                break;
            default:
                self.view.backgroundColor = UIColor.white
                break;
        }
    }
    
    
    @IBAction func next(_ sender: Any) {
        
        self.hideScreenItems(hide:true)
    }
    
    func hideScreenItems(hide:Bool) {
        
        self.qrCode?.isHidden = hide
        self.nextBtn?.isHidden = hide
        self.colorSelect?.isHidden = hide
        self.brightness?.isHidden = hide
        self.brightnessLabel?.isHidden = hide
    }
}

