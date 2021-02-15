/*
Basic ABC/interface to implement effect
*/

#ifndef EFFECT_HPP
#define EFFECT_HPP
class device; //fwd declare this since it's circular

class effect {
public:
	virtual void apply(device& d) = 0;
	virtual void deactivate(device& d) = 0;
};
#endif // !EFFECT_HPP
