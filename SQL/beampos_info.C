#include "/home/jbane/headers/SQLanalysis.h"
#include "/home/jbane/headers/rootalias.h"
#include "/home/jbane/headers/inc1.h"


void beampos_info(int run, int debug=1)
{
  TSQLServer* Server = TSQLServer::Connect( mysql_connection.Data(), mysql_user.Data(), mysql_password.Data() );
	TString query = Form( "select beam_x_pos, beam_y_pos, beam_x_size, beam_y_size from MARATHONanalysis where run_number=%d;", run);
	TSQLResult* result = Server->Query(query.Data());
  if(result->GetRowCount()==0) {
    cout << "Run " << run<< "  not found!!"<<"\n"; exit(1);}	

  TSQLRow *row = result->Next();

  float beam_x_pos, beam_y_pos, beam_x_size, beam_y_size;
  if(row->GetField(0)==nullptr){
    beam_x_pos=-1000.0;
  }  else{
    beam_x_pos = atof(row->GetField(0));
  }
  if(row->GetField(1)==nullptr){
    beam_y_pos=-1000.0;
  }  else{
    beam_y_pos = atof(row->GetField(1));
  }
  if(row->GetField(2)==nullptr){
    beam_x_size=-1000.0;
  }  else{
    beam_x_size = atof(row->GetField(2));
  }
  if(row->GetField(3)==nullptr){
    beam_y_size=-1000.0;
  }
  else{
    beam_y_size = atof(row->GetField(3));
  }

  vector<double> beampos_info = beam_info(run,debug);
  //sizex,sizey,xpos ypos
  float check = beampos_info[0];


  if( beam_x_size == -1000.0 || beam_x_size!=check || beam_y_size != beampos_info[1])
  {
    if(debug) cout <<"Updating run "<< run <<endl;

    TString update = Form( "Update MARATHONanalysis SET beam_x_size = %f, beam_y_size=%f, beam_x_pos=%f, beam_y_pos=%f where run_number=%d", beampos_info[0], beampos_info[1], beampos_info[2], beampos_info[3], run );
    Server->Query(update.Data());
  }
  else{cout << "Not updating run "<<run <<endl;}


  Server->Close();

}
