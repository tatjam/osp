#pragma once
#include "../surface_provider_node.h"
#include <glm/glm.hpp>

// idname = Math
class MathSPN : public SurfaceProviderNode
{
public:

	bool show_warning;

	SurfaceProvider* surf;

	enum MathOperation
	{
		ADD,			// R = A + B
		SUBSTRACT,		// R = A - B
		MULTIPLY,		// R = A * B
		DIVIDE,			// R = A / B
		MODULO,			// R = A % B
		POWER,			// R = A ^ B	
		MAX,			// R = max(A, B)
		MIN,			// R = min(A, B)
		ABS,			// R = abs(A) (B is ignored)
		SIGN,			// R = sign(A) (B is ignored)
			
		END_MARKER
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(MathOperation,
		{
		{ADD, "add"},
		{SUBSTRACT, "substract"},
		{MULTIPLY, "multiply"},
		{DIVIDE, "divide"},
		{MODULO, "modulo"},
		{POWER, "power"},
		{MAX, "max"},
		{MIN, "min"},
		{ABS, "abs"},
		{SIGN, "sign"},
		});


	MathOperation operation;

	glm::vec3 val_a;
	glm::vec3 val_b;

	ValueType valtype_a = ValueType::V1;
	ValueType valtype_b = ValueType::V1;

	static constexpr int INPUT_A = 0;
	static constexpr int INPUT_B = 1;
	static constexpr int OUTPUT = 2;

	virtual bool do_imgui(int id);
	virtual std::string get_name() override;
	virtual void process(size_t length) override;
	virtual void create(SurfaceProvider * surf) override;


	virtual nlohmann::json serialize() override;
	virtual void deserialize(nlohmann::json j) override;
};