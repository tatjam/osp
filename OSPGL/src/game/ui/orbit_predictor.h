#pragma once
#include "../../orbital/newton_body.h"
#include "../../orbital/space_system.h"
#include "../../render/renderlow/drawables/dmodel.h"

#include <list>
#include <deque>
#include <mutex> 

struct OrbitSnapshot
{
	// position relative to inertial frame
	glm::dvec3 pos;
	// velocity relative to inertial frame
	glm::dvec3 delta;

	double t;

	OrbitSnapshot(NewtonState st, double t)
	{
		this->pos = st.pos;
		this->delta = st.delta;
		this->t = t;
	}

	OrbitSnapshot() {}
};

struct OrbitSegment
{
	bool requires_mesh_update;

	// Number of points
	size_t point_count;
	// Time (since epoch) at first point
	double t0;
	// Delta-time (precision)
	double dt;

	std::vector<OrbitSnapshot> points;

	// If this is true the thread is still generating points
	bool thread_running;

	// TODO: Inertial frame
};

struct ReferenceFrame
{
	// Body (or barycenter) used as center
	SpaceBody* center;

	// TODO BELOW THIS POINT
	// Does the reference frame rotate with the body or is it fixed
	// with the inertial frame?
	bool rotating;
	// If this is non-null rotating will be ignored and
	// the frame of reference will rotate to
	// the line joining center and aligned
	SpaceBody* aligned;


	// Transforms a point to the reference frame
	glm::dvec3 transform(glm::dvec3 inertial, double t);
};

struct PredictorSettings
{
	double past_points_time = 86400 * 10;
	double future_points_time = 86400 * 10;
	double predictor_dt = 10;

	// Only points every [x] are rendered
	int future_precision = 1000;
};

class OrbitPredictor
{
private:

	size_t past_prev_points = 0;
	size_t future_prev_points = 0;

	DModel past_mesh;
	DModel future_mesh;



public:

	bool predicting = true;

	// In seconds
	PredictorSettings def_predictor_sets;

	SpaceSystem* system;

	ReferenceFrame def_frame;

	size_t uid = 0;

	std::mutex mtx;
	std::deque<OrbitSnapshot> def_past;
	bool def_future_clear;
	std::deque<OrbitSnapshot> def_future;
	std::thread* def_thread;

	std::unordered_map<size_t, OrbitSegment> segments;

	bool show_ui = false;

	double thruster_clear_time = 0.25;
	double thruster_clear_current = 0;

	// Closes all threads
	void close_threads();
	// Opens all threads
	void open_threads();

	// Creates and launches a new segment simulator.
	// link is the segment we are based off, if 0 it's the def_predictor
	// link_t is the base time we are based off, if out of bounds the closest point will be used
	// dvel is the acceleration applied at that point relative to inertial frame (TODO: Maybe non-instant acceleration?)
	// Returns the ID of the new segment
	int add_segment(size_t link, int link_t, double dt, size_t points, glm::dvec3 dvel, bool threaded = true);

	// Deletes the orbit segment
	// If the thread is working it will be stopped before removing the segment.
	void remove_segment(size_t id);

	// Called by space_system every "tick".
	// dt is real dt, used to remove old points
	void update(double dt, double t, NewtonState state);

	// Forcefully predicts some steps, used to help the user
	// when maneouvering, it's meant to be a relatively lower quality
	// prediction as it would otherwise lag the game, but the user
	// can choose it via the UI
	// state is the vessel's state after applying acceleration
	void force_predict(NewtonState state, size_t steps, double dt, double t);

	// Updates the GPU mesh for both
	// past and future.
	// If the ammount of points changed it's more expensive
	// as it has to re-allocate memory.
	void update_mesh();

	void draw(glm::mat4 view, glm::mat4 proj);

	// Copy constructor as we need to keep
	// threads
	OrbitPredictor(const OrbitPredictor& b);
	OrbitPredictor(SpaceSystem* system);
	~OrbitPredictor();
};

