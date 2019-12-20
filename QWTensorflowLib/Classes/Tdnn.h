#ifndef DNNVAD_TDNN_H_
#define DNNVAD_TDNN_H_

#include <string>

namespace Dnnvad
{

int LoadDnnModle(std::string model_path);
 
class TdnnTfWrapper
{
public:

TdnnTfWrapper();

~TdnnTfWrapper();

int Create();

int Run(float *input, int frames, int featdim, int* predicted);

private:

    void* mTf;
};

} // namespace Dnnvad

#endif // DNNVAD_TDNN_H_
