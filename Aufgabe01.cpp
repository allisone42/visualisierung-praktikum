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
            Options( fantom::Options::Control& control ) : DataAlgorithm::Options(control)
            {
                add< int >("Number of houses", "Specify how many houses you want", 1);
            }
        };

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs(fantom::DataOutputs::Control& control) : DataAlgorithm::DataOutputs(control)
            {
                add< Grid< 3 > >("grid");
            }
        };

        Aufgabe01( InitData& data ) : DataAlgorithm( data )
        {
            //initialize internal data members
        }

        void execute( const Algorithm::Options& options, const volatile bool& /*abortFlag*/ ) override
        {
            size_t numberOfCells = 4;

            // floor
            Tensor<double, 3> p0(0.0, 0.0, 0.0);
            Tensor<double, 3> p1(1.0, 0.0, 0.0);
            Tensor<double, 3> p2(1.0, 1.0, 0.0);
            Tensor<double, 3> p3(0.0, 1.0, 0.0);
            // level
            Tensor<double, 3> p4(0.0, 1.0, 1.0);
            Tensor<double, 3> p5(1.0, 1.0, 1.0);
            Tensor<double, 3> p6(1.0, 0.0, 1.0);
            Tensor<double, 3> p7(0.0, 0.0, 1.0);
            // roof
            Tensor<double, 3> p8(0.5, 0.5, 1.5);
            // flag
            Tensor<double, 3> p9(0.5, 0.5, 1.7);
            Tensor<double, 3> p10(0.5, 0.7, 1.66);
            Tensor<double, 3> p11(0.5, 0.5, 1.6);

            std::vector<Tensor<double, 3>> myHouse;

            myHouse.push_back(p0);
            myHouse.push_back(p1);
            myHouse.push_back(p2);
            myHouse.push_back(p3);
            myHouse.push_back(p4);
            myHouse.push_back(p5);
            myHouse.push_back(p6);
            myHouse.push_back(p7);
            myHouse.push_back(p8);
            myHouse.push_back(p9);
            myHouse.push_back(p10);
            myHouse.push_back(p11);

            for (std::vector<Tensor<double,3 > >::const_iterator i = myHouse.begin(); i != myHouse.end(); ++i)
                debugLog() << *i << ' ' ; // shows me the tensors

            std::shared_ptr< const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(std::move(myHouse), Precision::UINT64);

            std::vector< size_t > pointsToDraw({0,1,2,3,4,5,6,7, 7,6,5,4,8, 8,9, 9,11,10});

            std::pair<Cell::Type, size_t> cellCounts[numberOfCells];

            cellCounts[0] = std::pair<Cell::Type, size_t> (Cell::HEXAHEDRON, 1);
            cellCounts[1] = std::pair<Cell::Type, size_t> (Cell::PYRAMID, 1);
            cellCounts[2] = std::pair<Cell::Type, size_t> (Cell::LINE, 1);
            cellCounts[3] = std::pair<Cell::Type, size_t> (Cell::TRIANGLE, 1);

            std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, numberOfCells, cellCounts, pointsToDraw);
            setResult("grid", myGrid);

            debugLog() << std::endl;

        }
    };
    AlgorithmRegister< Aufgabe01 > dummy( "VisPraktikum/Aufgabe01", "Tiny Houses." );
}
