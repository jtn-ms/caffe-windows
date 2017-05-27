#ifndef CAFFE__LAYERS_REGISTRY_HPP_
#define CAFFE__LAYERS_REGISTRY_HPP_

#include "caffe/common.hpp"  
#include "caffe/layers/concat_layer.hpp"  
#include "caffe/layers/conv_layer.hpp" 
#include "caffe/layers/detection_output_layer.hpp"  
#include "caffe/layers/dropout_layer.hpp"  
#include "caffe/layers/input_layer.hpp"  
#include "caffe/layers/flatten_layer.hpp"  
#include "caffe/layers/inner_product_layer.hpp"  
#include "caffe/layers/relu_layer.hpp" 
#include "caffe/layers/reshape_layer.hpp"  
#include "caffe/layers/normalize_layer.hpp"
#include "caffe/layers/permute_layer.hpp"
#include "caffe/layers/prior_box_layer.hpp"
#include "caffe/layers/pooling_layer.hpp"  
#include "caffe/layers/lrn_layer.hpp"  
#include "caffe/layers/softmax_layer.hpp"  
// may be need to include other head files  

namespace caffe
{
	extern INSTANTIATE_CLASS(InputLayer);
	extern INSTANTIATE_CLASS(InnerProductLayer);
	extern INSTANTIATE_CLASS(DropoutLayer);
	extern INSTANTIATE_CLASS(ConcatLayer);
	extern INSTANTIATE_CLASS(ConvolutionLayer);
	REGISTER_LAYER_CLASS(Convolution);
	extern INSTANTIATE_CLASS(ReLULayer);
	REGISTER_LAYER_CLASS(ReLU);
	extern INSTANTIATE_CLASS(PoolingLayer);
	REGISTER_LAYER_CLASS(Pooling);
	extern INSTANTIATE_CLASS(LRNLayer);
	REGISTER_LAYER_CLASS(LRN);
	extern INSTANTIATE_CLASS(SoftmaxLayer);
	REGISTER_LAYER_CLASS(Softmax);
	extern INSTANTIATE_CLASS(NormalizeLayer);
	extern INSTANTIATE_CLASS(PermuteLayer);
	extern INSTANTIATE_CLASS(FlattenLayer);
	extern INSTANTIATE_CLASS(PriorBoxLayer);
	extern INSTANTIATE_CLASS(ReshapeLayer);
	extern INSTANTIATE_CLASS(DetectionOutputLayer);
	// other layer  
	// may be need to REGISTER_LAYER_CREATOR()  
}
#endif //CAFFE__LAYERS_REGISTRY_HPP_