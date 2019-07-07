#include "ipostoffice.h"
#include "postoffice.h"

std::shared_ptr<IPostOffice> IPostOffice::create(unsigned gate_count)
{
    auto post = std::make_shared<PostOffice>(gate_count);
    return post;
}
