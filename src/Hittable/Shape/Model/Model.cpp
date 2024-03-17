#include "Model.h"

Model::Model(const std::string &model_filename, std::shared_ptr<Material> _material) : material(_material)
{
    LoadModel(model_filename);
}

bool Model::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    return objects->IsHit(ray, t_range, hit_record);
}

void Model::LoadModel(const std::string &model_filename)
{
    std::string file_path = "models/" + model_filename;
    std::ifstream file;

    for (size_t i = 0; i < 5; i++)
    {
        file.open(file_path);
        if (file)
        {
            break;
        }
        file_path = "../" + file_path;
    }

    if (!file)
    {
        std::cerr << "ERROR: Could not load model file '" << model_filename << "'.\n";
    }

    file.ignore(1);

    size_t vertices_size, faces_size;
    file >> vertices_size >> faces_size;

    vertices.reserve(vertices_size);

    std::vector<std::shared_ptr<Hittable>> faces;
    faces.reserve(faces_size);

    Interval model_ranges[3] = {{_INFINITY, -_INFINITY}, {_INFINITY, -_INFINITY}, {_INFINITY, -_INFINITY}};

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        if (token == "v")
        {
            Point3 point;
            iss >> point[0] >> point[1] >> point[2];

            for (size_t i = 0; i < 3; i++)
            {
                model_ranges[i].min = std::fmin(model_ranges[i].min, point[i]);
                model_ranges[i].max = std::fmax(model_ranges[i].max, point[i]);
            }

            vertices.emplace_back(point[0], point[1], point[2]);
        }
        else if (token == "f")
        {
            break;
        }
    }

    double max_interval = 0;

    for (size_t i = 0; i < 3; i++)
    {
        max_interval = std::fmax(max_interval, model_ranges[i].max - model_ranges[i].min);
    }

    double k = 300. / max_interval;
    double b[3] = {0. - k * model_ranges[0].min, 0. - k * model_ranges[1].min, 0. - k * model_ranges[2].min};

    auto transform =
        [k, b](Point3 &point)
    {
        for (size_t i = 0; i < 3; i++)
        {
            point[i] = k * point[i] + b[i];
        }

        return point;
    };

    for (auto &vertex : vertices)
    {
        transform(vertex.point);
    }

    file.seekg(-100, std::ios::cur);
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "f")
        {
            std::string v1, v2, v3;
            size_t index1, index2, index3;

            iss >> v1 >> v2 >> v3;
            std::istringstream(v1.substr(0, v1.find('/'))) >> index1;
            std::istringstream(v2.substr(0, v2.find('/'))) >> index2;
            std::istringstream(v3.substr(0, v3.find('/'))) >> index3;

            index1--;
            index2--;
            index3--;

            auto mesh = std::make_shared<Mesh>(index1, index2, index3, *this);

            auto normal = mesh->face.Normal();
            vertices[index1].normal += normal;
            vertices[index2].normal += normal;
            vertices[index3].normal += normal;

            faces.push_back(mesh);
        }
    }

    for (auto &vertex : vertices)
    {
        vertex.normal = UnitVector(vertex.normal);
    }

    bounding_box = AABB(transform(Point3{model_ranges[0].min, model_ranges[1].min, model_ranges[2].min}),
                        transform(Point3{model_ranges[0].max, model_ranges[1].max, model_ranges[2].max}));

    objects = std::make_shared<BVH>(faces, 0, faces.size());
}
