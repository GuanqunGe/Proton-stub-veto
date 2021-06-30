/* This standalone analyze the clusters in event-basis, and create a new root TTree that save the maximum BDT score of clusters
 * in 1 event for BDT train test file; if one event has no clusters, then the maximum BDT score will be assigned to be -0.1.
 *
 * new TTree(root file) will be saved under the same dir as the vertexed train test file
 *
 * June 29th, 2021, by Guanqun.
 */
#include <stdexcept>
#include <algorithm>

void Add_Event_Max_BDTscore(){

    std::vector<std::string> Threshold = {"25"};
    std::vector<std::string> MinPts = {"1", "2", "3", "4", "5", "6", "7", "8"};  //# of neighbor pts
    std::vector<std::string> Eps = {"1", "2", "3", "4", "5", "6", "7", "8"};   //# neighbor distance, cm
    //std::vector<std::string> MinPts = {"1"};  //# of neighbor pts
    //std::vector<std::string> Eps = {"1"};   //# neighbor distance, cm

    std::string cut="min_impact_parameter_to_shower < 1e5";  //cut on training and testing clusters

    //path for the BDT test file
    std::string test_file_path = "/uboone/data/users/gge/SinglePhotonAnalysis/vertexed_file/Mar2021_lowproton_seaview_test/v08_00_00_43/vertex/mcc9.1_v08_00_00_26_run3_G_reco2/ProtonBDT/";
    //path for the BDT training file
    std::string train_input_path="/uboone/data/users/gge/SinglePhotonAnalysis/vertexed_file/Mar2021_lowproton_seaview_test/v08_00_00_43/vertex/v08_00_00_26_run1_reco2/ProtonBDT/";
    //path for the BDT output file
    std::string train_output_path="/uboone/app/users/gge/hellstroms_hive/hive/working_directory/ProtonBDT_May2021/SEAview_configuration/train_run1_test_run3/";


    for(const auto &th: Threshold){
	for(const auto &pt: MinPts){
	    for(const auto &d:Eps){

		std::string tag=pt+"pts_"+d+"cm_"+th+"ADC";
		std::cout << "On tag: " << tag << std::endl;


		TFile* ftest_cluster = new TFile((test_file_path+"merged_vertexed_tree_flat_clusters_"+tag+".root").c_str(), "read");
		TTree* ttest_cluster = (TTree*)ftest_cluster->Get("vertex_tree");
		TFile* ftest_event = new TFile((test_file_path+"merged_vertexed_singlephoton_hist_"+tag+".root").c_str(), "read");
		TTree* ttest_event = (TTree*)ftest_event->Get("singlephotonana/vertex_tree");

		TFile* ftest_output = new TFile((train_output_path+tag+"/XGBoost_train_output_Proton_BDTProton.root").c_str(), "read");
		TTree* t_train_s = (TTree*)ftest_output->Get("t_stest");
		TTree* t_train_b = (TTree*)ftest_output->Get("t_btest");

		//create a new TFile, which in future could be used as a friend of the vertex tree
		TFile* ftest_event_maxBDTscore = new TFile((test_file_path+"updated_merged_vertexed_singlephoton_hist_"+tag+".root").c_str(), "recreate");
		TTree* ttest_event_maxBDTscore = new TTree("BDTscore", "Tree of max BDT score of event");

		try{
		    if((t_train_s->GetEntries() + t_train_b->GetEntries()) != ttest_cluster->GetEntries(cut.c_str())){
			std::ostringstream os;
			os << "Number of clusters do not match!! \nSignal Train cluster: " << t_train_s->GetEntries()  << ", Background Train cluster: " << t_train_b->GetEntries()  << "Total cluster in flat tree: " << ttest_cluster->GetEntries(cut.c_str());
			throw std::runtime_error(os.str());
		    }
		}
		catch(const std::runtime_error &e){
		    std::cout << e.what() << std::endl;
		}


		//iterator through all event entries
		int sss_num_candidates;
		int run_number;
		int subrun_number;
		int event_number;
		ttest_event->SetBranchAddress("sss_num_candidates", &sss_num_candidates);
		ttest_event->SetBranchAddress("run_number", &run_number);
		ttest_event->SetBranchAddress("subrun_number", &subrun_number);
		ttest_event->SetBranchAddress("event_number", &event_number);

		//iterate through all cluster entries
		int pdg; 
		double min_impact_parameter_to_shower;
		ttest_cluster->SetBranchAddress("pdg", &pdg);
		ttest_cluster->SetBranchAddress("min_impact_parameter_to_shower", &min_impact_parameter_to_shower);

		//BDT score
		double sig_test, bkg_test;
		t_train_b->SetBranchAddress("bkg_test", &bkg_test);
		t_train_s->SetBranchAddress("sig_test", &sig_test);

		//max BDT score of each event
		double max_BDT_score;
		ttest_event_maxBDTscore->Branch("run_number", &run_number, "run_number/I");
		ttest_event_maxBDTscore->Branch("subrun_number", &subrun_number, "subrun_number/I");
		ttest_event_maxBDTscore->Branch("event_number", &event_number, "event_number/I");
		ttest_event_maxBDTscore->Branch("sss_num_candidates", &sss_num_candidates, "sss_num_candidates/I");
		ttest_event_maxBDTscore->Branch("max_bdt_score", &max_BDT_score, "max_bdt_score/D");

		//iterate through all the event and all clusters
		for(int event_entry = 0, cluster_entry=0, strain_entry=0, btrain_entry =0; event_entry != ttest_event->GetEntries(); ++event_entry){
		//for(int event_entry = 0, cluster_entry=0, strain_entry=0, btrain_entry =0; event_entry < 10000 ; ++event_entry){


		    if(event_entry % 10000 == 0) std::cout << "On entry " << event_entry << "/" << ttest_event->GetEntries() << " Cluster entry "<< cluster_entry << "/" << ttest_cluster->GetEntries() << std::endl;

		    ttest_event->GetEntry(event_entry);


		    max_BDT_score = -0.1; //for event with no cluster, its max BDT score will be -0.1
		    //loop over clusters within this event
		    for(int dummy = 0; dummy!= sss_num_candidates; ++dummy){
			//insert checks for the entry numbers, tho if everything is right, this should alway be false.
			if(cluster_entry >= ttest_cluster->GetEntries()){
                             std::ostringstream os;
			     os << "cluster entry exceeds the total entries: " << cluster_entry << " " << ttest_cluster->GetEntries();
			     throw std::runtime_error(os.str());	
			}
			ttest_cluster->GetEntry(cluster_entry++);
			if(min_impact_parameter_to_shower >= 1e5) continue;

			if(strain_entry >= t_train_s->GetEntries() && btrain_entry >=  t_train_b->GetEntries()){
			    std::ostringstream os;
			    os << "train test entry exceeds the total entries, signal: " << strain_entry << " " << t_train_s->GetEntries() << ", background: " << btrain_entry << " " << t_train_b->GetEntries();
			    throw std::runtime_error(os.str());
			}
			if(pdg == 2212){ //if it's a proton cluster
			    t_train_s->GetEntry(strain_entry++);
			    max_BDT_score = std::max(max_BDT_score, sig_test);
			}else{
			    t_train_b->GetEntry(btrain_entry++);
			    max_BDT_score = std::max(max_BDT_score, bkg_test);
			}
		    }
		    
		    //fill the tree
		    ttest_event_maxBDTscore->Fill();	
		}
	

		ftest_cluster->Close();
		ftest_event->Close();
		ftest_output->Close();

		ftest_event_maxBDTscore->cd();
		ttest_event_maxBDTscore->Write();
		ftest_event_maxBDTscore->Close();


	    }
	}
    }

    return;
}
