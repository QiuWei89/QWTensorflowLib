
#include "Tdnn.h"
#include <vector>

//#include "tensorflow/cc/ops/const_op.h"
//#include "tensorflow/cc/ops/image_ops.h"
//#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
//#include "tensorflow/cc/ops/array_ops.h"

#include <Eigen/Core>
#include <Eigen/Dense>

namespace Dnnvad
{

class Tdnntf
{
public:
    Tdnntf();
    ~Tdnntf();
    static int LoadModle(std::string model_path);

    int Create();

    int Run(float *input, int frames, int featdim, int predicted[]);

private:
    static tensorflow::GraphDef mGraphdef; //Graph Definition for current model

    tensorflow::Session *mSession;
};

tensorflow::GraphDef Tdnntf::mGraphdef;

Tdnntf::Tdnntf() : mSession(NULL)
{
}

Tdnntf::~Tdnntf()
{
    if (mSession)
    {
        delete mSession;
    }

    mSession = NULL;
}

int Tdnntf::LoadModle(std::string model_path)
{

    std::cout << "INFO: Tdnntf::LoadModle..." << std::endl;


//    std::string model_path = "frozen_model.pb";

    tensorflow::Status status_load = ReadBinaryProto(tensorflow::Env::Default(), model_path, &mGraphdef); //从pb文件中读取图模型;
    if (!status_load.ok())
    {
        std::cout << "ERROR: Loading model failed..." << model_path << std::endl;
        std::cout << status_load.ToString() << "\n";
        return -1;
    }

    std::cout << "INFO: Loading model successed..." << std::endl;

    return 0;
}

int Tdnntf::Create()
{
    std::cout << "INFO: Tdnntf::Create..." << std::endl;
    tensorflow::Status status = tensorflow::NewSession(tensorflow::SessionOptions(), &mSession); //创建新会话Session

    if (!status.ok())
    {
        std::cout << "ERROR: new a session failed..." << status.ToString() << std::endl;
        return -1;
    }

    tensorflow::Status status_create = mSession->Create(mGraphdef); //将模型导入会话Session中;
    if (!status_create.ok())
    {
        delete mSession;
        mSession = NULL;
        std::cout << "ERROR: Creating graph in session failed..." << status_create.ToString() << std::endl;
        return -1;
    }

    std::cout << "INFO: tf create successed..." << std::endl;
    return 0;
}

int Tdnntf::Run(float *input, int frames, int featdim, int predicted[])
{
    int rows = frames, cols = featdim;
    // 转换到Eigen::TensorMap，三个参数依次为：类型：float，维度：2， 存储模式：行优先
    auto mapped_X_ = Eigen::TensorMap<Eigen::Tensor<float, 2, Eigen::RowMajor>>(input, rows, cols);

    // 再转换到Eigen::Tensor，参数同上
    auto eigen_X_ = Eigen::Tensor<float, 2, Eigen::RowMajor>(mapped_X_);

    tensorflow::Tensor X_(tensorflow::DT_FLOAT, tensorflow::TensorShape({rows, cols}));

    X_.tensor<float, 2>() = eigen_X_;

    std::vector<tensorflow::Tensor> outputs;
    std::string output_node = "nihao";
    tensorflow::Status status_run = mSession->Run({{"frame_batch", X_}}, {output_node}, {}, &outputs);

    if (!status_run.ok())
    {
        std::cout << "ERROR: RUN failed..." << std::endl;
        std::cout << status_run.ToString() << "\n";
        return -1;
    }

    tensorflow::Tensor t = outputs[0];       // Fetch the first tensor
    int ndim2 = t.shape().dims();            // Get the dimension of the tensor
    int output_dim1 = t.shape().dim_size(1); // Get the target_class_num from 1st dimension
    int output_dim0 = t.shape().dim_size(0);

    if (ndim2 != 2 || output_dim0 != rows || output_dim1 != 2)
    {
        std::cout << "ERROR: OUTPUT tensor dim:" << ndim2 << " dim0:" << output_dim0 << " dim1" << output_dim1 << std::endl;
        std::cout << "ERROR: OUTPUT tensor failed..." << std::endl;
        return -1;
    }
    auto tmap = t.tensor<float, 2>(); // Tensor Shape: [batch_size, target_class_num]

    for (int i = 0; i < output_dim0; i++)
    {
        predicted[i] = tmap(i, 0) > tmap(i, 1) ? 0 : 1;
    }

    return 0;
}

int LoadDnnModle(std::string model_path)
{
    return Tdnntf::LoadModle(model_path);
}

TdnnTfWrapper::TdnnTfWrapper() : mTf(NULL)
{
}

TdnnTfWrapper::~TdnnTfWrapper()
{
    if(mTf)
    {
        Tdnntf *tf = new Tdnntf;
        delete tf;
        mTf = NULL;
    }
}

int TdnnTfWrapper::Create()
{
    Tdnntf *tf = new Tdnntf;

    if (tf->Create() != 0)
    {
        delete tf;

        return -1;
    }

    mTf = (void *)tf;

    return 0;
}

int TdnnTfWrapper::Run(float *input, int frames, int featdim, int *predicted)
{
    Tdnntf *tf = (Tdnntf *)mTf;
    if (tf == NULL)
    {
        return -1;
    }

    return tf->Run(input, frames, featdim, predicted);
}

} // namespace Dnnvad
