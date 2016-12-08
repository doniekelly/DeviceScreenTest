//
//  openingViewController.swift
//  Screen Capture
//
//  Created by Donie Kelly on 24/11/2016.
//  Copyright © 2016 Inhance Technology. All rights reserved.
//

import UIKit

class openingViewController: UIViewController {

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
    }
    
    override func viewDidAppear(_ animated: Bool) {
        
        super.viewDidAppear(animated)
        #if CLIENT
            self.performSegue(withIdentifier: "showClient", sender: nil)
        #else
            self.performSegue(withIdentifier: "showTarget", sender: nil)
        #endif
        
    }
}
