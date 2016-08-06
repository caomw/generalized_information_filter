/*
 * PairwiseResidual.hpp
 *
 *  Created on: Aug 6, 2016
 *      Author: Bloeschm
 */

#ifndef GIF_PAIRWISERESIDUAL_HPP_
#define GIF_PAIRWISERESIDUAL_HPP_

#include "generalized_information_filter/common.hpp"
#include "generalized_information_filter/Model.hpp"

namespace GIF{

class PairwiseResidual: public Model{
 public:
  PairwiseResidual(StateDefinition* stateDefinition, StateDefinition* noiseDefinition, StateDefinition* residualDefinition):
    Model(residualDefinition,{stateDefinition, stateDefinition, noiseDefinition}){
    meas_ = nullptr;
  };
  virtual ~PairwiseResidual(){};
  virtual void eval(const State* inPre, const State* inPost, const State* noise, State* res) = 0;
  virtual void jacPre(const State* inPre, const State* inPost, MXD& out) = 0;
  virtual void jacPost(const State* inPre, const State* inPost, MXD& out) = 0;
  virtual void jacNoise(const State* inPre, const State* inPost, MXD& out) = 0;
  virtual void _eval(const std::vector<const State*>& in, State* out){
    eval(in[0],in[1],in[2],out);
  }
  virtual void _jac(const std::vector<const State*>& in, MXD& out, int c){
    switch(c){
      case 0:
        jacPre(in[0],in[1],out);
        break;
      case 1:
        jacPost(in[0],in[1],out);
        break;
      case 2:
        jacNoise(in[0],in[1],out);
        break;
      default:
        assert(false);
    }
  }
  void jacPreFD(const State* inPre, const State* inPost, MXD& out, const double& delta){
    const std::vector<const State*> inVec({inPre, inPost, nullptr});
    _jacFD(inVec,out,0,delta);
  }
  void jacPostFD(const State* inPre, const State* inPost, MXD& out, const double& delta){
    const std::vector<const State*> inVec({inPre, inPost, nullptr});
    _jacFD(inVec,out,1,delta);
  }
  void jacNoiseFD(const State* inPre, const State* inPost, MXD& out, const double& delta){
    const std::vector<const State*> inVec({inPre, inPost, nullptr});
    _jacFD(inVec,out,2,delta);
  }
  virtual void preProcess(const State* inPre, const State* inPost, const MXD& cov){};
  virtual void postProcess(const State* inPre, const State* inPost, const MXD& cov){};
  void setMeas(const State* meas){
    meas_ = meas;
  }

 private:
  const State* meas_;
};

}

#endif /* GIF_PAIRWISERESIDUAL_HPP_ */