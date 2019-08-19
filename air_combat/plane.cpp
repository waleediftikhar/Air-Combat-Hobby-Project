#include "plane.hpp"

void Plane::move_plane(float x, float y) {

	if (body_->GetPosition()(1) < 0)
		return;
	b2Vec2 vel = body_->GetLinearVelocity();
	//Keep plane flying on the same level only if it has enough speed
	if (x != 0) {
		if (vel(1) < 0)
			y = 0;
		if (vel(0) < (max_speed_ / 2) && vel(0) > -(max_speed_ / 2))
			y = 0;
	}
	//Check max_speed and apply 0-force if it has been reached
	if (x > 0) {
		if (vel(0) > max_speed_)
			x = 0;
	}
	else if (x < 0) {
		if (vel(0) < -max_speed_)
			x = 0;
	}
	//Allow to go up only if enough speed
	if (x == 0 && y == -1) {
		if (vel(0) < 2 * max_speed_ / 3 && vel(0) > -(2 * max_speed_ / 3))
			y = 0;
	}
	b2Vec2 dir = b2Vec2(5 * x*acceleration_, 3 * y*acceleration_);
	body_->ApplyForce(dir, body_->GetWorldCenter(), 1);
}

bool Plane::shoot(b2World *world) {
	if (bullets_.size() < 5) {
		b2BodyDef bodydef;
		bodydef.type = b2_dynamicBody;
		bodydef.bullet = true;
		b2Vec2 pos = body_->GetPosition();
		b2Vec2 vel = body_->GetLinearVelocity();
		//directional indicators
		int x = 1;
		int y = 1;

		if (vel(0) < 0) {
			x = -1;
			y = -1;
		}
		float ang = body_->GetAngle();

		bodydef.position.Set(pos(0) + 5 * x*std::cos(ang), pos(1) + 5 * y*std::sin(ang));
		bodydef.angle = 0;

		b2PolygonShape box;
		box.SetAsBox(0.01, 0.01);

		b2FixtureDef bodyfixture;
		bodyfixture.shape = &box;
		bodyfixture.density = 0.05;

		b2Body *bullet = world->CreateBody(&bodydef);
		bullet->CreateFixture(&bodyfixture);

		bullet->ApplyLinearImpulse(b2Vec2(70 * x*std::cos(ang), 70 * y*std::sin(ang)), bullet->GetWorldCenter(), 1);

		bullets_.push_back(bullet);
		bulletcounters_.push_back(0);
		return true;
	}
	return false;
}

std::list<b2Body*>& Plane::get_bullets() {
	return bullets_;
}
std::list<int>& Plane::get_counters() {
	return bulletcounters_;
}
/*void Plane::deletebullet(b2Body& bullet)
{

	auto x = bulletcounters_.begin();
	for (auto i : bullets_) {
		if (bullet == *i) {
			break;
		}
		x++;
	}
	bullets_.remove(bullet);
	bulletcounters_.remove(*x);
}
*/

std::string& Plane::get_type() {
	return type_;
}

std::string& Weapon::get_type() {
	return type_;
}

b2Body* Plane::get_body() {
	return body_;
}

int Plane::change_hp(int n) {
	hp_ += n;
	return hp_;
}

int Plane::get_hp() {
	return hp_;
}
