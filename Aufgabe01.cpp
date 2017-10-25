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
            //int numberOfPoints = 8;
            //int tensorField[numberOfPoints];

            // points for hexahedron
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


            std::vector<Tensor<double, 3>> myHexahedron;

            myHexahedron.push_back(p0);
            myHexahedron.push_back(p1);
            myHexahedron.push_back(p2);
            myHexahedron.push_back(p3);
            myHexahedron.push_back(p4);
            myHexahedron.push_back(p5);
            myHexahedron.push_back(p6);
            myHexahedron.push_back(p7);
            myHexahedron.push_back(p8);

            //for (std::vector<Tensor<double,3 > >::const_iterator i = myHexahedron.begin(); i != myHexahedron.end(); ++i)
            //    debugLog() << *i << ' ' ; // shows me the tensors

            std::shared_ptr< const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(std::move(myHexahedron), Precision::UINT64);

            std::vector< size_t > pointsToDraw({0,1,2,3,4,5,6,7, 7,6,5,4,8});

            std::pair<Cell::Type, size_t> cellCounts[1];
            cellCounts[0] = std::pair<Cell::Type, size_t> (Cell::HEXAHEDRON, 1);
            cellCounts[1] = std::pair<Cell::Type, size_t> (Cell::PYRAMID, 1);

            std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, 2, cellCounts, pointsToDraw);
            setResult("grid", myGrid);

            debugLog() << std::endl;

        }
    };
    AlgorithmRegister< Aufgabe01 > dummy( "VisPraktikum/Aufgabe01", "Tiny Houses." );
}
