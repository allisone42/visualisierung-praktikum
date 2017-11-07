#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/math.hpp>
#include <fantom/fields.hpp>
#include <fantom/dataset/ValueArray.hpp>

using namespace fantom;

namespace
{
    class Aufgabe01 : public DataAlgorithm
    {
        //bool isAutoRun = true; //reloads options when changes occur

        public:
            struct Options : public DataAlgorithm::Options
            {
                Options(fantom::Options::Control & control)
                    : DataAlgorithm::Options(control)
                {
                    addTab("Basic");
                    add<int>("Number of houses", "Specify how many houses you want (at least 4)", 4, &acceptNumberOfHouses);
                }

                static int acceptNumberOfHouses( const int& i )
                {
                    int number = std::max(i, 0);

                    //check if number typed in was at least 4
                    if (number >= 4)
                        return number;
                    else {
                        return 4;
                    }
                }
            };

            struct DataOutputs : public DataAlgorithm::DataOutputs
            {
                DataOutputs(fantom::DataOutputs::Control& control) : DataAlgorithm::DataOutputs(control)
                {
                    add<Grid<3>>("grid");
                }
            };

            Aufgabe01(InitData & data)
                : DataAlgorithm(data)
            {
                // initialize internal data members
            }

            void execute(const Algorithm::Options& options, const volatile bool& /*abortFlag*/) override
            {
                //getting number of houses from users options
                int numberOfHouses = options.get< int >("Number of houses");

                //distance between the houses
                const double DISTANCE = 3.0;

                //cell types are hexadedron, pyramid, line, triangle
                const size_t NUMBEROFCELLTYPES = 4;

                //vector consisting of data point Tensors
                std::vector<Tensor<double, 3>> points;

                //indices of points
                std::vector<size_t> indices;

                //create levels (hexadedrons)
                for (int i = 0; i < numberOfHouses; i++) {
                    int shift = i * DISTANCE;
                    points = createHexadedron(points, shift);
                    for(int j = 0; j < 8; j++) {
                        indices.push_back(j + (i * 8));
                    }
                }

                //create roofs (pyramids)
                for (int i = 0; i < numberOfHouses; i++) {
                    int shift = i * DISTANCE;

                    Tensor<double, 3> p8(0.5, 0.5 + shift, 1.5);

                    points.push_back(p8);

                    for(int j = 4 + (i * 8); j < (8 + (i * 8)); j++) {
                        indices.push_back(j);
                    }
                    indices.push_back(8 * numberOfHouses + i);
                }

                //create poles (lines)
                for (int i = 0; i < numberOfHouses; i++) {
                    int shift = i * DISTANCE;

                    Tensor<double, 3> p9(0.5, 0.5 + shift, 1.7);

                    points.push_back(p9);

                    indices.push_back(8 * numberOfHouses + i);
                    indices.push_back(9 * numberOfHouses + i);
                }

                //create flags (triangles)
                for (int i = 0; i < numberOfHouses; i++) {
                    int shift = i * DISTANCE;

                    Tensor<double, 3> p10(0.5, 0.5 + shift, 1.6);
                    Tensor<double, 3> p11(0.5, 0.7 + shift, 1.66);

                    points.push_back(p10);
                    points.push_back(p11);

                    indices.push_back(9 * numberOfHouses + i);
                    indices.push_back(points.size() - 2);
                    indices.push_back(points.size() - 1);
                }

                std::pair<Cell::Type, size_t> cellCounts[NUMBEROFCELLTYPES];
                size_t houses = (size_t) numberOfHouses;

                std::pair<Cell::Type, size_t> hexas(Cell::HEXAHEDRON, houses);
                std::pair<Cell::Type, size_t> pyramids(Cell::PYRAMID, houses);
                std::pair<Cell::Type, size_t> lines(Cell::LINE, houses);
                std::pair<Cell::Type, size_t> triangles(Cell::TRIANGLE, houses);

                cellCounts[0] = hexas;
                cellCounts[1] = pyramids;
                cellCounts[2] = lines;
                cellCounts[3] = triangles;

                // create 3 dimensional Domain
                std::shared_ptr< const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(points, Precision::UINT64);

                // create Grid and fill it with domain, all data points and indices
                std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, NUMBEROFCELLTYPES, cellCounts, indices);

                // show grid
                setResult("grid", myGrid);
            }

        private:

            std::vector<Tensor<double, 3>> createHexadedron(std::vector<Tensor<double, 3>> points, int shift)
            {
                // floor
                Tensor<double, 3> p0(0.0, 0.0 + shift, 0.0);
                Tensor<double, 3> p1(1.0, 0.0 + shift, 0.0);
                Tensor<double, 3> p2(1.0, 1.0 + shift, 0.0);
                Tensor<double, 3> p3(0.0, 1.0 + shift, 0.0);

                // level
                Tensor<double, 3> p4(0.0, 1.0 + shift, 1.0);
                Tensor<double, 3> p5(1.0, 1.0 + shift, 1.0);
                Tensor<double, 3> p6(1.0, 0.0 + shift, 1.0);
                Tensor<double, 3> p7(0.0, 0.0 + shift, 1.0);

                points.push_back(p0);
                points.push_back(p1);
                points.push_back(p2);
                points.push_back(p3);
                points.push_back(p4);
                points.push_back(p5);
                points.push_back(p6);
                points.push_back(p7);
                return points;
            }
        };

    AlgorithmRegister< Aufgabe01 > dummy( "VisPraktikum/Aufgabe01", "Tiny Houses." );
}
