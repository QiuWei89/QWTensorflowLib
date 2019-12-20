//
//  MLDecoderSDK.m
//  MLDecoderSDK
//
//  Created by 邱威 on 2019/12/9.
//  Copyright © 2019 qiuwei. All rights reserved.
//

#import "MLDecoderSDK.h"
#include "Tdnn.h"

@implementation MLDecoderSDK

- (void)loadModel:(NSString *)modelPath
{
    std::string model_path = (std::string)[modelPath fileSystemRepresentation];
    int status = Dnnvad::LoadDnnModle(model_path);
    if (status == 0) {
        NSLog(@"model load success");
    }
    NSLog(@"good job!");
}

@end
