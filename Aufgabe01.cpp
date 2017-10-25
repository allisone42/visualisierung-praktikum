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
            Tensor<double, 3> p0(1.0, 0.0, 0.0);
            Tensor<double, 3> p1(0.0, 1.0, 0.0);
            Tensor<double, 3> p2(0.0, 0.0, 1.0);

            std::vector<Tensor<double,3>> myTriangle;
            myTriangle.push_back(p0);
            myTriangle.push_back(p1);
            myTriangle.push_back(p2);

            for (std::vector<Tensor<double,3 > >::const_iterator i = myTriangle.begin(); i != myTriangle.end(); ++i)
                debugLog() << *i << ' ' ; // shows me the tensors

            std::shared_ptr< const DiscreteDomain< 3 > > myDomain = DomainFactory::makeDomainArbitrary(std::move(myTriangle), Precision::UINT64);

            std::vector< size_t > pointsToDraw({0,1,2});

            std::pair<Cell::Type, size_t> cellCounts[1];
            cellCounts[0] = std::pair<Cell::Type, size_t> (Cell::TRIANGLE,1);

            std::shared_ptr< const Grid< 3 > > myGrid = DomainFactory::makeGridUnstructured( *myDomain, 1, cellCounts, pointsToDraw);
            setResult("grid", myGrid);

            debugLog() << std::endl;

        }
    };
    AlgorithmRegister< Aufgabe01 > dummy( "VisPraktikum/Aufgabe01", "Tiny Houses." );
}
