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
                    add<int>("Number of houses", "Specify how many houses you want (at least 4)", 4, &getNumberOfHouses);
                }

                static int getNumberOfHouses( const int& i )
                {
                    int number = std::max(i, 0);

                    //check if number typed in was at least 4, if not, set 4
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

                //cell types are hexadedron, pyramid, line, triangle
                const size_t NUMBEROFCELLTYPES = 4;

                //vector consisting of data point Tensors
                std::vector<Tensor<double, 3>> points;

                //indices of points
                std::vector<size_t> indices;

                //create levels (hexadedrons)
                for (int i = 0; i < numberOfHouses; i++) {
                    points = addPoints("hexadedron", points, i);

                    for(int j = 0; j < 8; j++) {
                        indices.push_back(j + (i * 8));
                    }
                }

                //create roofs (pyramids)
                for (int i = 0; i < numberOfHouses; i++) {
                    points = addPoints("pyramid", points, i);

                    for(int j = 4 + (i * 8); j < (8 + (i * 8)); j++) {
                        indices.push_back(j);
                    }
                    indices.push_back(8 * numberOfHouses + i);
                }

                //create poles (lines)
                for (int i = 0; i < numberOfHouses; i++) {
                    points = addPoints("line", points, i);

                    indices.push_back(8 * numberOfHouses + i);
                    indices.push_back(9 * numberOfHouses + i);
                }

                //create flags (triangles)
                for (int i = 0; i < numberOfHouses; i++) {
                    points = addPoints("triangle", points, i);

                    indices.push_back(9 * numberOfHouses + i);
                    indices.push_back(points.size() - 2);
                    indices.push_back(points.size() - 1);
                }

                std::pair<Cell::Type, size_t> cellCounts[NUMBEROFCELLTYPES];

                cellCounts[0] = {Cell::HEXAHEDRON, numberOfHouses};
                cellCounts[1] = {Cell::PYRAMID, numberOfHouses};
                cellCounts[2] = {Cell::LINE, numberOfHouses};
                cellCounts[3] = {Cell::TRIANGLE, numberOfHouses};

                // create 3 dimensional Domain
                std::shared_ptr< const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(points, Precision::UINT64);

                // create Grid and fill it with domain, all data points and indices
                std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, NUMBEROFCELLTYPES, cellCounts, indices);

                // show grid
                setResult("grid", myGrid);
            }

        private:

            std::vector<Tensor<double, 3>> addPoints(std::string cellType, std::vector<Tensor<double, 3>> points, int houseIndex)
            {
                //distance between the houses
                double DISTANCE = 3.0;

                int shift = houseIndex * DISTANCE;
                if (cellType == "hexadedron") {
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
                }

                else if (cellType == "pyramid") {
                    Tensor<double, 3> p8(0.5, 0.5 + shift, 1.5);
                    points.push_back(p8);
                }

                else if (cellType == "line") {
                    Tensor<double, 3> p9(0.5, 0.5 + shift, 1.7);
                    points.push_back(p9);
                }

                else if (cellType == "triangle") {
                    Tensor<double, 3> p10(0.5, 0.5 + shift, 1.6);
                    Tensor<double, 3> p11(0.5, 0.7 + shift, 1.66);

                    points.push_back(p10);
                    points.push_back(p11);
                }

                return points;
            }
        };

    AlgorithmRegister< Aufgabe01 > dummy( "VisPraktikum/Aufgabe01", "Tiny Houses." );
}
