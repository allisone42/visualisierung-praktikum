#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
#include <fantom/graphics.hpp>

using namespace fantom;

namespace
{
    /**
     * @brief The VisThresholdAlgorithm class
     *
    */
    class VisThresholdAlgorithm : public VisAlgorithm
    {
        std::unique_ptr< Primitive > mGlyphs;

    public:
        struct Options : public VisAlgorithm::Options
        {
            Options( Control& control ) : VisAlgorithm::Options( control )
            {
                add< TensorFieldDiscrete < Scalar > >("Field", "Input Field" );
                add< Color >("Color", "Drawing Color", Color(0.75, 0.75, 0.0));
                add< double >("Threshold", "Threshold/limit", 0.0008);
                add< double >("Radius", "Radius of sphere", 0.15);
                add< bool >("Scale", "Scale size of points from their values", false);
            }
        };

        struct VisOutputs : public VisAlgorithm::VisOutputs
        {
            VisOutputs( fantom::VisOutputs::Control& control ) : VisAlgorithm::VisOutputs( control )
            {
                addGraphics( "Spheres" );
            }
        };

        VisThresholdAlgorithm( InitData & data) : VisAlgorithm( data )
        {
        }

        void execute( const Algorithm::Options& options, const volatile bool& /* abortFlag */ ) override
        {
            //make sphere data adjustable
            mGlyphs = getGraphics("Spheres").makePrimitive();
            std::shared_ptr< const TensorFieldDiscrete < Scalar > > field = options.get< TensorFieldDiscrete < Scalar > >("Field");

            //abort if no field is given yet
            if (!field) {
                return;
            }

            //save points to different lists, depending on their sizes
            std::unordered_map<double, std::vector<Point3>> valuesToDraw;
            std::unique_ptr< TensorFieldDiscrete< Scalar >::DiscreteEvaluator > evaluator = field->makeDiscreteEvaluator();
            auto threshold = options.get< double >("Threshold");

            std::shared_ptr< const Grid< 3 > > grid = std::dynamic_pointer_cast< const Grid< 3 > >( field->domain() );
            const ValueArray< Point3 >& points = grid->points();

            auto size = points.size();
            for (size_t i = 0; i < size; i++) {
                auto value = evaluator->value(i);
                if (value[0] >= threshold) {
                    double diameter = value[0] / threshold;
                    //create list for specific diameter if necessary
                    if (valuesToDraw.find(diameter) == valuesToDraw.end()) {
                        valuesToDraw.insert({diameter, std::vector<Point3>()});
                    }
                    valuesToDraw[diameter].push_back(points[i]);
                }
            }

            Color color = options.get< Color >("Color");
            double radius = options.get< double >("Radius");
            double radiusToDraw;
            for (auto& p : valuesToDraw) {
                if (options.get< bool >("Scale"))
                {
                    radiusToDraw = p.first * radius;
                }
                else
                {
                    radiusToDraw = radius;
                }

                mGlyphs->add(Primitive::POINTS).setPointSize(radiusToDraw).setColor(color).setVertices(p.second);
            }
        }

    };

    AlgorithmRegister< VisThresholdAlgorithm > reg("VisPraktikum/Aufgabe03", "Markiert Schwellwerte");
}
