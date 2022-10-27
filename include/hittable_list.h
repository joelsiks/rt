
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) {
        add(object);
    }

    void clear() {
        m_objects.clear();
    }
    void add(std::shared_ptr<Hittable> object) {
        m_objects.push_back(object);
    }

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec)
    const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;
};

bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for(const auto& object : m_objects) {
        if(object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif // HITTABLE_LIST_H