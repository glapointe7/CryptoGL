#include "../UnitTests/TestContainer.hpp"

#include "../UnitTests/Adler32Test.hpp"
#include "../UnitTests/AffineTest.hpp"
#include "../UnitTests/AdfgvxTest.hpp"
#include "../UnitTests/AESTest.hpp"
#include "../UnitTests/Base64Test.hpp"
#include "../UnitTests/BeaufortGermanTest.hpp"
#include "../UnitTests/BeaufortTest.hpp"
#include "../UnitTests/BlakeTest.hpp"
#include "../UnitTests/BlowfishTest.hpp"
#include "../UnitTests/CAST128Test.hpp"
#include "../UnitTests/CAST256Test.hpp"
#include "../UnitTests/CaesarTest.hpp"
#include "../UnitTests/CamelliaTest.hpp"
#include "../UnitTests/ChaoTest.hpp"
#include "../UnitTests/CollonTest.hpp"
#include "../UnitTests/DESTest.hpp"
#include "../UnitTests/DelastelleTest.hpp"
#include "../UnitTests/FleissnerTest.hpp"
#include "../UnitTests/FourSquaresTest.hpp"
#include "../UnitTests/GronsfeldTest.hpp"
#include "../UnitTests/HC256Test.hpp"
#include "../UnitTests/HellmanMerkleKnapsackTest.hpp"
#include "../UnitTests/HillTest.hpp"
#include "../UnitTests/IDEATest.hpp"
#include "../UnitTests/KeccakTest.hpp"
#include "../UnitTests/LRCTest.hpp"
#include "../UnitTests/MD2Test.hpp"
#include "../UnitTests/MD4Test.hpp"
#include "../UnitTests/MD5Test.hpp"
#include "../UnitTests/MorseTest.hpp"
#include "../UnitTests/Mysty1Test.hpp"
#include "../UnitTests/NihilistesTest.hpp"
#include "../UnitTests/NoekeonTest.hpp"
#include "../UnitTests/PlayfairTest.hpp"
#include "../UnitTests/PolybeTest.hpp"
#include "../UnitTests/RC2Test.hpp"
#include "../UnitTests/RC4Test.hpp"
#include "../UnitTests/RC5Test.hpp"
#include "../UnitTests/RC6Test.hpp"
#include "../UnitTests/RabbitTest.hpp"
#include "../UnitTests/RailfenceTest.hpp"
#include "../UnitTests/RedefenceTest.hpp"
#include "../UnitTests/RedefenceZigzagTest.hpp"
#include "../UnitTests/Ripemd128Test.hpp"
#include "../UnitTests/Ripemd160Test.hpp"
#include "../UnitTests/Ripemd256Test.hpp"
#include "../UnitTests/Ripemd320Test.hpp"
#include "../UnitTests/RozierTest.hpp"
#include "../UnitTests/SEALTest.hpp"
#include "../UnitTests/SHA1Test.hpp"
#include "../UnitTests/SHA224Test.hpp"
#include "../UnitTests/SHA256Test.hpp"
#include "../UnitTests/SHA384Test.hpp"
#include "../UnitTests/SHA512Test.hpp"
#include "../UnitTests/SHA512_224Test.hpp"
#include "../UnitTests/SHA512_256Test.hpp"
#include "../UnitTests/Salsa20Test.hpp"
#include "../UnitTests/ScreamTest.hpp"
#include "../UnitTests/SkipjackTest.hpp"
#include "../UnitTests/Snow3GTest.hpp"
#include "../UnitTests/TigerTest.hpp"
#include "../UnitTests/TranspositionColumnsTest.hpp"
#include "../UnitTests/TranspositionDoubleTest.hpp"
#include "../UnitTests/TranspositionRowsTest.hpp"
#include "../UnitTests/TripleDESTest.hpp"
#include "../UnitTests/TwoSquaresTest.hpp"
#include "../UnitTests/TwofishTest.hpp"
#include "../UnitTests/UnsortAlphaTest.hpp"
#include "../UnitTests/VigenereMultTest.hpp"
#include "../UnitTests/VigenereTest.hpp"
#include "../UnitTests/WhirlpoolTest.hpp"
#include "../UnitTests/WolseleyTest.hpp"
#include "../UnitTests/XTEATest.hpp"
//#include "../testing/SerpentTest.hpp"

#include "../UnitTests/StringMethodsTest.hpp"
#include "../UnitTests/VectorMethodsTest.hpp"
#include "../UnitTests/SortTest.hpp"
#include "../UnitTests/QuickSortTest.hpp"
//#include "Graph.hpp"





struct Vertex;
 
struct Edge 
{
    int v1;
    int v2;
};
 
struct Vertex 
{
    int label;
    // 'squashed' is a list of any vertices squashed into this one during the
    // random contraction
    std::vector<int> squashed;
    std::vector<int> edges;
};
 
struct Graph 
{
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    int vertexCount;
};
 
Graph loadGraph(std::ifstream &infile) 
{
    Graph g;
    g.vertexCount = 0;

    std::string line;

    while (std::getline(infile, line)) 
    {
        std::stringstream ss(line);
        // make a new Vertex for this line
        Vertex v;
        int vertexNum;
        ss >> vertexNum;
        v.label = vertexNum;
        int edgeNum;
        while (ss >> edgeNum) 
        {
            v.edges.push_back(edgeNum);
            // only record the edge once, so when edgeNum > vertexNum
            if (edgeNum > vertexNum) 
            {
                Edge e;
                e.v1 = vertexNum;
                e.v2 = edgeNum;
                g.edges.push_back(e);
            }
        }
        g.vertices.push_back(v);
        g.vertexCount++;
    }

    return g;
}
 
void randomContraction(Graph &g) 
{
    // pick a random edge
    Edge e = g.edges[rand() % g.edges.size()];
    // now contract the vertices joined by this edge. we'll merge in the edges
    // from v2 into v1 and remove v2 from the graph.
    Vertex v1 = g.vertices[e.v1-1];
    Vertex v2 = g.vertices[e.v2-1];

    // build a new list of edges based on the contracted graph
    std::vector<Edge> newEdges;
    for (std::vector<Edge>::iterator i = g.edges.begin(); i < g.edges.end(); i++) 
    {
        Edge newEdge = *i;
        if (newEdge.v1 == v2.label) 
        {
            // re assign edge to point to v1
            newEdge.v1 = v1.label;
        }
        if (newEdge.v2 == v2.label) 
        {
            newEdge.v2 = v1.label;
        }
        // only add it to the new list if it's not a self-loop
        if (newEdge.v1 != newEdge.v2) 
        {
            newEdges.push_back(newEdge);
        } 
    }
    g.edges = newEdges;

    // null out the vertex to signify that it's been squashed
    g.vertices[e.v1-1].squashed.push_back(g.vertices[e.v2-1].label);
    g.vertices[e.v1-1].squashed.insert(g.vertices[e.v1-1].squashed.begin(), v2.squashed.begin(), v2.squashed.end());
    g.vertices[e.v2-1].label = 0;
    g.vertexCount--;
}
 
// compute the MinCut by making repeated attempts at the random contraction algorithm
int kargerMinCut(Graph &g) 
{
    // randomContraction has >= P(1/n^2) chance of finding the min cut. Run it a sufficient
    // number of times so that we have only a 1/n chance of failure.
    int attempts = g.vertices.size() * g.vertices.size() * log(g.vertices.size());
    Graph bestCutGraph;
    int bestCutCount = g.edges.size();

    std::cout << "Making " << attempts << " attempts" << std::endl;

    for (int i = 0; i < attempts; i++) 
    {
        // make a copy of g for contracting
        Graph g2 = g;
        // seed the RNG
        srand(time(NULL));
        std::cout << "\r" << i;
        // use random contraction to reduce the graph to a cut.
        while (g2.vertexCount > 2) 
        {
            randomContraction(g2);
        }

        if (g2.edges.size() < bestCutCount) 
        {
            // this is our best so far
            bestCutGraph = g2;
            bestCutCount = g2.edges.size();
            std::cout << std::endl << "New best... " << bestCutCount << std::endl;
        }
    }

    // return the best cut we found
    g = bestCutGraph;
    return bestCutCount;
} 






int main(int argc, char **argv)
{
//    std::ifstream infile("/media/gabriel/Data/Algorithms Analysis/kargerMinCut.txt");
//    Graph g = loadGraph(infile);
// 
//    // attempt to find the min cut
//    int minCut = kargerMinCut(g);
// 
//    std::cout << "Min cut: " << minCut << std::endl;
//
//    return 0; 
        
    
//    std::ifstream in("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/QuickSort.txt");
//    
//    // test file open.   
//    if (in) 
//    {        
//        uint32_t value;
//        UInt32Vector Data;
//        Data.reserve(10000);
//        
//        // read the elements in the file into a vector.  
//        while (in >> value) 
//        {
//            Data.push_back(value);
//        }
//        QuickSort Q(Data, QuickSortPivotChoice::Median);
//        std::cout << "Total inversions: " << Q.sort(0, 9999);
//    }
    //100   615      587      518
    //1000 10297    10184    8921
    //10000 162085   164123  138382
    UnitTests::TestContainer::runAllTests();
}