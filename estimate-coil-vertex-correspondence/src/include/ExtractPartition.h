#ifndef __EXTRACT_PARTITION_H__
#define __EXTRACT_PARTITION_H__
#include <vector>
#include <stdexcept>

class ExtractPartition{

public:

ExtractPartition(const std::vector< std::vector<unsigned int> > combinations) : combinations(combinations) {

  // use by default only 1 partition
  this->partitionAmount = 1;
  this->partitionSize = combinations.size();

}

ExtractPartition& set_partition_amount(const int& amount) {

  if( amount < 0 ) {

    throw std::runtime_error("Negative amount of partitions not allowed.");

  }

  this->partitionAmount = amount;

  compute_partition_size();

  return *this;

}

std::vector< std::vector<unsigned int> > get_partition(const int& index) const {

  if(index < 1) {

    throw std::runtime_error("Index smaller 1 not allowed.");

  }

  if(index > this->partitionAmount) {

    throw std::runtime_error("Index larger than partition amount.");

  }

  // compute offset
  const int offset = (index - 1) * this->partitionSize;

  // special case code for last partition -> might be larger to include all
  // missing values
  if( index == this->partitionAmount ) {

    return std::vector< std::vector<unsigned int> > ( this->combinations.begin() + offset, this->combinations.end() );

  }
  else{

    return std::vector< std::vector<unsigned int> > ( this->combinations.begin() + offset, this->combinations.begin() + offset + partitionSize );

  }

}

private:

void compute_partition_size() {

  this->partitionSize = (double) this->combinations.size() / this->partitionAmount;

}

std::vector< std::vector<unsigned int> > combinations;
int partitionAmount = 1;
double partitionSize = 1;

};
#endif
