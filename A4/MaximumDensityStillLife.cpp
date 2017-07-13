
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class MaximumDensityStillLife : public Script {
private:
  /*
   *  Boolean variables for the cells.
   */
  BoolVarArray cells;
  
  /*
   * 3-by-3 slice of a maximum-density pattern
   */ 
  IntVarArray sliceOfMDP;
  
  /*
   * number of live cells
   */ 
  IntVar noOfLives;
  int board_size;
  
public:

    MaximumDensityStillLife(const SizeOptions& os) :
    cells(*this,(os.size()+4)*(os.size()+4),0,1), sliceOfMDP(*this,pow(ceil(os.size()/3.0),2),0,6), noOfLives(*this,0,pow(os.size(), 2))
    {
        //board size plus a border of size two around the pattern
        this -> board_size = os.size()+4;
        
          Matrix<BoolVarArgs> matrix(cells, board_size, board_size);

          rel(*this, sum(cells) == noOfLives);
          
//          constraint on border of size two around the pattern and set it to empty.
          linear(*this, matrix.slice(0, 2, 0, board_size), IRT_EQ, 0);// First 2 rows
          linear(*this, matrix.slice(board_size-2, board_size, 0, board_size), IRT_EQ, 0);// Last 2 rows        
          linear(*this, matrix.slice(2, board_size-2, 0, 2), IRT_EQ, 0);// First 2 columns         
          linear(*this, matrix.slice(2, board_size-2, board_size-2, board_size), IRT_EQ, 0);// Last 2 columns
          
          int blockNo = 0;
          for (int col = 2; col < board_size - 2; col++) {
              for (int row = 2; row < board_size - 2 ; row++) {
                  
                  // neighbours of cell(col, row)
                  BoolVarArgs neighbours = getNeighbours(col, row);
                  
                  BoolVar two_neighbours(*this,0,1);
                  BoolVar three_neighbours(*this,0,1);
                  BoolVar still_live(*this,0,1);
                  
                  /* still life constraints:
                   *    A live cell with two or three live neighbours is alive in the next generation.
                   *    A dead cell must not have 3 neighbours to stay dead in next generation
                   */ 
                  linear(*this,neighbours,IRT_EQ,2,two_neighbours);
                  linear(*this,neighbours,IRT_EQ,3,three_neighbours);
                  rel(*this,two_neighbours,BOT_OR,three_neighbours,still_live); 
                  rel(*this, (matrix(col,row) >> still_live));
                  rel(*this, (!matrix(col,row) >> !three_neighbours));
                  
                  /*constraint on 3*3 blocks
                   *    the maximum number of lives cells is 6.
                   */
                  if (col % 3 == 2 && row % 3 == 2) {
                      linear(*this, matrix.slice(col, col+3, row, row+3), IRT_EQ, sliceOfMDP[blockNo]);
                      blockNo++;
                  }
              }
              
//              constraint on the inner-most border cells to have less than 3 neighbours,so they will stay dead
              linear(*this,matrix.slice (2, 3, col, col+3), IRT_LE, 3);
              linear(*this,matrix.slice (col, col+3, 2, 3), IRT_LE, 3);
              linear(*this,matrix.slice (board_size-3, board_size-2, col, col+ 3), IRT_LE, 3);
              linear(*this,matrix.slice (col, col+3, board_size-3, board_size-2), IRT_LE, 3);
          }
           
          branch(*this, cells, INT_VAR_NONE(), INT_VAL_MAX());
          branch(*this, noOfLives, INT_VAL_SPLIT_MAX());
    }
    
    BoolVarArgs getNeighbours(int col, int row){
        BoolVarArgs neighbours;
        Matrix<BoolVarArgs> matrix(cells, board_size, board_size);
        neighbours << matrix(col-1,row-1)<< matrix(col,row-1)<< matrix(col+1,row-1)// above neighbours
                << matrix(col-1,row)<< matrix(col+1,row) // side neighbours
                << matrix(col-1,row+1) << matrix(col,row+1)<< matrix(col+1,row+1);//below neighbours
        return neighbours;
    }

    MaximumDensityStillLife(bool share, MaximumDensityStillLife& mdsl) : Script(share,mdsl) {
        cells.update(*this, share, mdsl.cells);
        sliceOfMDP.update(*this, share, mdsl.sliceOfMDP);
        noOfLives.update(*this, share, mdsl.noOfLives);
    }
    
    // search for a best solution
    virtual void constrain (const Space& s) {
        const MaximumDensityStillLife& mdsl = static_cast<const MaximumDensityStillLife&>(s);
        rel(*this,noOfLives>mdsl.noOfLives);
        rel(*this,sum(sliceOfMDP) > sum (mdsl.sliceOfMDP));
    }
    
    virtual Space* copy(bool share) {
        return new MaximumDensityStillLife(share,*this);
    }
    
    virtual void print(std::ostream& p) const {
        
        std::cout << "Number of lives: " << noOfLives << "\n";
        int boardSize=sqrt(cells.size());
        Matrix<BoolVarArgs> matrix(cells, boardSize, boardSize);
        
        for (int c = 2; c < boardSize-2; c++) {
            for (int l = 2; l < boardSize-2; l++){
                p <<"-+-";
            }
            p << std::endl;
            for (int r = 2; r < boardSize-2; r++) {
                p << matrix(c,r) << "| ";
            }
            p << std::endl;
        }
        p <<"******************************" ;
    }
};


int main(int argc, char* argv[]) {
  SizeOptions so("Maximum Density Still Life");
  so.size(8);
  so.solutions(0);
  so.parse(argc,argv);
  
//  Script::run<MaximumDensityStillLife,BAB,SizeOptions>(so);
//  return 0;
  
   MaximumDensityStillLife* mdsl = new MaximumDensityStillLife(so);
   
   BAB<MaximumDensityStillLife> bab(mdsl);
   delete mdsl;
   while (MaximumDensityStillLife* q = bab.next()){
       q->print(std::cout);
       std::cout << std::endl;
       std::cout<<"depth: "<<bab.statistics().depth<<std::endl;
       std::cout<<"node: "<<bab.statistics().node<<std::endl;
       std::cout<<"propagation: "<<bab.statistics().propagate<<std::endl;
       std::cout<<"failures: "<<bab.statistics().fail<<std::endl;
       std::cout<<"Memory: "<<bab.statistics().memory<<std::endl<<std::endl;
       std::cout<<"///////////////////////"<<std::endl;
       delete q;
   }
   return 0;
}