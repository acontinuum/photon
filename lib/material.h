#ifndef MATERIAL_H
#define MATERIAL_H

class material {
	public:
		virtual ~material() = default; // Deconstructor

		virtual bool scatter(const ray& in, const hit_record& rec, color& attentuation, ray& scattered) const {
			return false;
		}

		virtual color emitted() {
			return color(0, 0, 0);
		}
};

class lambertian : public material {
	public:
		lambertian(const color& albedo) : albedo(albedo) {}

		bool scatter(const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered) const override {
			auto scatter_direction = rec.normal + random_unit_vector();
			if (scatter_direction.near_zero())
			            scatter_direction = rec.normal;
			scattered = ray(rec.p, scatter_direction);
			attentuation = albedo;
			return true;
		}	
	private:
		color albedo;
};	

class metal : public material {
	public:
		metal(const color& albedo, double roughness) : albedo(albedo), roughness(roughness) {}

		bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
			vec3 reflected = reflect(r_in.direction(), rec.normal);
			reflected = unit_vector(reflected) + (roughness * random_unit_vector());
			scattered = ray(rec.p, reflected);
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0); //make sure that roughness isnt flipping the direction into sphere ie misconfig, rough > 1.
		}
	private:
		color albedo;
		double roughness;
};

class dielectric : public material {
	public:
		dielectric(double refraction_index) : refraction_index(refraction_index) {}

		bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
			attenuation = color(1, 1, 1);
			double ri = rec.front_face ? (1/refraction_index) : refraction_index; // TODO ?

			vec3 unit_direction = unit_vector(r_in.direction());
			
	        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
	        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

			bool cannot_refract = ri * sin_theta > 1;
			vec3 direction;

			if (cannot_refract || reflectance(cos_theta, ri) > random_double(0, 1))
	            direction = reflect(unit_direction, rec.normal);
	        else
	            direction = refract(unit_direction, rec.normal, ri);

	
	        scattered = ray(rec.p, direction);
	        
			return true;
		}	
		
	private:
		double refraction_index; // Ratio of material to media material is in. IE air, water, vacuum.
	
		static double reflectance(double cosine, double refraction_index) {
		    // Use Schlick's approximation for reflectance.
		    auto r0 = (1 - refraction_index) / (1 + refraction_index);
		    r0 = r0*r0;
		    return r0 + (1-r0)*std::pow((1 - cosine),5);
		}
};

class diffuse_light : public material {
	public:
		diffuse_light(const color& emit) : emit(emit) {}

		virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
			return false;
		}

		virtual color emitted() {
			return emit;
		}

	private:
		color emit;
};


#endif
