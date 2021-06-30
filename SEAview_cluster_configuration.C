/* This standalone studies how overall BDT performance changes on an event-basis, with different SEAview configuration parameters,
 * namely, different parameters for (SEAviewHitThreshold; SEAviewDBSCANMinPts; SEAviewDBSCANEps)
 *
 * May 25th, 2021, by Guanqun.
 */
#include <stdexcept>
#include <algorithm>

void Draw2DHist(TCanvas* c, TH2D* h, std::string printout="TEST.pdf"){
    c->cd();
    c->Clear();
    h->SetMaximum(h->GetMaximum() * 1.2);
    h->Draw("colz");
    c->Print(printout.c_str());
    return;
}

bool Is_proton_event(const std::vector<int> &daughter_pdgs){
    for(auto &pdg : daughter_pdgs){
	if(pdg == 2212) return true;
    }
    return false;
}

bool IsSignal(int num_reco_track, int mctruth_num_exiting_proton){
    return num_reco_track == 0 && mctruth_num_exiting_proton > 0;
}

bool IsBackground(int num_reco_track, int mctruth_num_exiting_proton){
    return num_reco_track == 0 && mctruth_num_exiting_proton == 0;
}

bool IsBackground(int mctruth_num_exiting_proton){
    return mctruth_num_exiting_proton ==0;
}

void SEAview_cluster_configuration(){

    std::vector<std::string> Threshold = {"25"};
    std::vector<std::string> MinPts = {"1", "2", "3", "4", "5", "6", "7", "8"};  //# of neighbor pts
    std::vector<std::string> Eps = {"1", "2", "3", "4", "5", "6", "7", "8"};   //# neighbor distance, cm
    std::vector<double> auc_vec{0.054795, 0.053522, 0.052809, 0.051961, 0.051606, 0.05122, 0.051085, 0.051338, 
				0.063907, 0.064488, 0.064727, 0.06542, 0.066241, 0.066949, 0.068047, 0.068938, 
				0.054977, 0.060708, 0.064965, 0.069629, 0.073596, 0.076334, 0.080165, 0.082577, 
				0.046554, 0.056426, 0.063233, 0.071274, 0.076311, 0.079757, 0.085472, 0.089439, 
				0.050395, 0.055873, 0.062344, 0.069824, 0.074844, 0.079265, 0.085311, 0.0911, 
				0.057154, 0.05451, 0.061088, 0.066498, 0.07279, 0.076503, 0.081764, 0.087565, 
				0.064875, 0.054793, 0.059778, 0.063496, 0.069409, 0.073116, 0.079153, 0.084811, 
				0.065662, 0.055665, 0.058797, 0.061294, 0.06563, 0.069461, 0.075764, 0.081108};

/*    std::vector<std::string> MinPts = {"1"};  //# of neighbor pts
    std::vector<std::string> Eps = {"1", "2"};   //# neighbor distance, cm
    std::vector<double> auc_vec{0.043986, 0.052453};
*/
    std::string cut="min_impact_parameter_to_shower < 1e5";  //cut on training and testing clusters

    //path for the BDT test file
    std::string test_file_path = "/uboone/data/users/gge/SinglePhotonAnalysis/vertexed_file/Mar2021_lowproton_seaview_test/v08_00_00_43/vertex/mcc9.1_v08_00_00_26_run3_G_reco2/ProtonBDT/";
    //path for the BDT training file
    std::string train_input_path="/uboone/data/users/gge/SinglePhotonAnalysis/vertexed_file/Mar2021_lowproton_seaview_test/v08_00_00_43/vertex/v08_00_00_26_run1_reco2/ProtonBDT/";
    //path for the BDT output file
    std::string train_output_path="/uboone/app/users/gge/hellstroms_hive/hive/working_directory/ProtonBDT_May2021/SEAview_configuration/train_run1_test_run3/";
    std::string pdfname = "/uboone/data/users/gge/SinglePhotonAnalysis/learning_garbage/LowEnergyProtonStudy/SEAview_cluster_configuration_BDT_score.pdf";


    TFile* fout = new TFile("/uboone/data/users/gge/SinglePhotonAnalysis/learning_garbage/LowEnergyProtonStudy/SEAview_cluster_configuration.root", "recreate");
    TDirectory* dir = fout->GetDirectory("");
    TH2D* h_proton_portion = new TH2D("h_proton_portion", "Fraction of proton clusters; Min Neighbour Pts; Max Neighbour Dist/cm", 8, 1, 9, 8, 1, 9);
    TH2D* h_proton_perev = new TH2D("h_proton_perev", "Num of proton clusters per event; Min Neighbour Pts; Max Neighbour Dist/cm", 8, 1, 9, 8, 1, 9);
    TH2D* h_other_perev = new TH2D("h_other_perev", "Number of non-proton clusters per event; Min Neighbour Pts; Max Neighbour Dist/cm", 8, 1, 9, 8, 1, 9);
    TH2D* h_proton_other_ratio = new TH2D("h_proton_other_ratio", "Ratio of proton/non-proton clusters per event; Min Neighbour Pts; Max Neighbour Dist/cm", 8, 1, 9, 8, 1, 9);
    TH2D* h_auc = new TH2D("h_auc", "minimum AUC of test sample; Min Neighbour Pts; Max Neighbour Dist/cm", 8, 1, 9, 8, 1, 9);


    TCanvas* c = new TCanvas("c", "c", 900, 600);
    //c->SetLogz();
    gStyle->SetOptStat(0);
    c->Print((pdfname+"[").c_str());

    int auc_index = 0;
    for(const auto &th: Threshold){
	for(const auto &pt: MinPts){
	    for(const auto &d:Eps){

		std::string tag=pt+"pts_"+d+"cm_"+th+"ADC";
		std::cout << "On tag: " << tag << std::endl;

		// ----------- draw train file related variable ----------------
		TFile* ftrain_cluster = new TFile((train_input_path+"merged_vertexed_tree_flat_clusters_"+tag+".root").c_str(), "read");
		TTree* t_train_cluster = (TTree*)ftrain_cluster->Get("vertex_tree");
		TFile* ftrain_vertex = new TFile((train_input_path+"merged_vertexed_singlephoton_hist_"+tag+".root").c_str(), "read");
		TTree* t_train_vertex = (TTree*)ftrain_vertex->Get("singlephotonana/vertex_tree");

		int num_train_event = t_train_vertex->GetEntries();
                int num_train_cluster = t_train_cluster->GetEntries(cut.c_str());
                int num_proton_cluster = t_train_cluster->GetEntries(("pdg == 2212 &&" + cut).c_str());
                int num_other_cluster = t_train_cluster->GetEntries(("pdg!=2212 &&" + cut).c_str());

                h_proton_portion->Fill(std::stoi(pt), std::stoi(d), num_proton_cluster/(double)num_train_cluster);
                h_proton_perev->Fill(std::stoi(pt), std::stoi(d), num_proton_cluster/(double)num_train_event);
                h_other_perev->Fill(std::stoi(pt), std::stoi(d), num_other_cluster/(double)num_train_event);
		h_proton_other_ratio->Fill(std::stoi(pt), std::stoi(d), num_proton_cluster/(double)num_other_cluster);
                h_auc->Fill(std::stoi(pt), std::stoi(d), auc_vec[auc_index]);
//		std::cout << "Config: " << tag << ", Total clusers: " << num_train_cluster<< " | Proton cluster: " << num_proton_cluster << " | Other cluster: " << num_other_cluster << " | Proton Fraction: " << num_proton_cluster/(double)num_train_cluster<<  std::endl;		

		++auc_index;

		t_train_cluster=nullptr; t_train_vertex=nullptr;
		ftrain_cluster->Close(); ftrain_vertex->Close();
                // ----------- end of train file --------------------------------


		// ---------- draw test result ----------------------------------
		TFile* fin_cluster = new TFile((test_file_path+"merged_vertexed_tree_flat_clusters_"+tag+".root").c_str(), "read");
		TTree* tin_cluster = (TTree*)fin_cluster->Get("vertex_tree");
		TFile* fin_event = new TFile((test_file_path+"merged_vertexed_singlephoton_hist_"+tag+".root").c_str(), "read");
		TTree* tin_event = (TTree*)fin_event->Get("singlephotonana/vertex_tree");

		TFile* fin_train = new TFile((train_output_path+tag+"/XGBoost_train_output_Proton_BDTProton.root").c_str(), "read");
		TTree* t_train_s = (TTree*)fin_train->Get("t_stest");
		TTree* t_train_b = (TTree*)fin_train->Get("t_btest");


		try{
		    if((t_train_s->GetEntries() + t_train_b->GetEntries()) != tin_cluster->GetEntries(cut.c_str()))
			throw std::runtime_error("Number of clusters do not match!!");
		}
		catch(const std::runtime_error &e){
		    std::cout << e.what() << std::endl;
		    std::cout << "Signal Train cluster: " << t_train_s->GetEntries()  << ", Background Train cluster: " << t_train_b->GetEntries()  << std::endl;
		    std::cout << "Total cluster in flat tree: " << tin_cluster->GetEntries(cut.c_str()) << std::endl;
		}


		TH1D* h_BDT_signal = new TH1D(("h_BDT_signal_"+tag).c_str(), Form("Max BDT score of clusters, %s; Max BDT score; event entries", tag.c_str()), 100,0, 1);
		TH1D* h_BDT_bkg = new TH1D(("h_BDT_bkg_"+tag).c_str(), Form("Max BDT score of clusters, %s; Max BDT score; event entries", tag.c_str()), 100,0, 1);
	 	//h_BDT_signal->SetDirectory(0); h_BDT_bkg->SetDirectory(0); 
	 	h_BDT_signal->SetDirectory(dir); h_BDT_bkg->SetDirectory(dir); 


		//iterator through all event entries
		int sss_num_candidates;
		int mctruth_num_exiting_protons;
		int reco_asso_tracks;
		std::vector<int>* mctruth_daughters_pdg=nullptr;
		tin_event->SetBranchAddress("sss_num_candidates", &sss_num_candidates);
		tin_event->SetBranchAddress("reco_asso_tracks", &reco_asso_tracks);
		tin_event->SetBranchAddress("mctruth_num_exiting_protons", &mctruth_num_exiting_protons);
		tin_event->SetBranchAddress("mctruth_daughters_pdg", &mctruth_daughters_pdg);

		//iterate through all cluster entries
		int pdg; 
		double min_impact_parameter_to_shower;
		tin_cluster->SetBranchAddress("pdg", &pdg);
		tin_cluster->SetBranchAddress("min_impact_parameter_to_shower", &min_impact_parameter_to_shower);

		//BDT score
		double sig_test, bkg_test;
		t_train_b->SetBranchAddress("bkg_test", &bkg_test);
		t_train_s->SetBranchAddress("sig_test", &sig_test);

		//iterate through all the event and all clusters
		for(int event_entry = 0, cluster_entry=0, strain_entry=0, btrain_entry =0; event_entry != tin_event->GetEntries() &&  cluster_entry < tin_cluster->GetEntries() && (strain_entry < t_train_s->GetEntries() || btrain_entry <  t_train_b->GetEntries()) ; ++event_entry){
		//for(int event_entry = 0, cluster_entry=0, strain_entry=0, btrain_entry =0; event_entry < 10000 ; ++event_entry){
		    tin_event->GetEntry(event_entry);


		    //check if it's signal or bkg event, or neither
		    //bool signal_event = Is_proton_event(*mctruth_daughters_pdg);
		    //bool signal_event = (mctruth_num_exiting_protons == 0 ? false : true );
		    bool signal_event = IsSignal(reco_asso_tracks, mctruth_num_exiting_protons);
		    bool bkg_event = IsBackground(reco_asso_tracks, mctruth_num_exiting_protons); 
		    //bool bkg_event = IsBackground(mctruth_num_exiting_protons); 

		    double max_BDT_score = -0.1; //this is important, as we don't wnat to include event with no clusters
		    //loop over clusters within this event
		    for(int dummy = 0; dummy!= sss_num_candidates; ++dummy){
			tin_cluster->GetEntry(cluster_entry++);
			if(min_impact_parameter_to_shower >= 1e5) continue;
			if(pdg == 2212){ //if it's a proton cluster
			    t_train_s->GetEntry(strain_entry++);
			    max_BDT_score = std::max(max_BDT_score, sig_test);
			}else{
			    t_train_b->GetEntry(btrain_entry++);
			    max_BDT_score = std::max(max_BDT_score, bkg_test);
			}
		    }
		
		    if(signal_event) h_BDT_signal->Fill(max_BDT_score);
		    else if(bkg_event) h_BDT_bkg->Fill(max_BDT_score);
		    //if(event_entry % 10000 == 0) std::cout << "On Event Entry " << event_entry << "Cluster entry: " << strain_entry << " " << btrain_entry << ", is signal event: " << signal_event << ", max BDT score: " << max_BDT_score << std::endl;
		}
	

		std::cout << "Signal event within range: " << h_BDT_signal->Integral() << " | outside range: " << h_BDT_signal->Integral(0, 101) - h_BDT_signal->Integral() << " | ratio: " << h_BDT_signal->Integral(0, 101)/(double)h_BDT_signal->Integral() - 1 << "\nbackground event within range: " << h_BDT_bkg->Integral() << " outside range: " << h_BDT_bkg->Integral(0, 101) - h_BDT_bkg->Integral() << " | ratio: " << h_BDT_bkg->Integral(0, 101)/(double)h_BDT_bkg->Integral() - 1 << std::endl;

		fin_cluster->Close();
		fin_event->Close();
		fin_train->Close();


 		fout->cd();
		h_BDT_signal->Write(); h_BDT_bkg->Write();
		c->cd();
    	        c->Clear();
		c->SetLogy();
		TLegend* leg = new TLegend(0.6, 0.7, 0.88, 0.88);
		leg->SetBorderSize(0);
		//area normalize the distribution
		h_BDT_signal->Scale(1.0/h_BDT_signal->Integral()); h_BDT_bkg->Scale(1.0/h_BDT_bkg->Integral());
		h_BDT_signal->SetMaximum(std::max(h_BDT_signal->GetMaximum(), h_BDT_bkg->GetMaximum())*80.0);
		h_BDT_signal->SetLineColor(kBlue-9); h_BDT_bkg->SetLineColor(kRed -9);
		leg->AddEntry(h_BDT_signal, "proton event (true +1p, reco 0p)", "L");
		leg->AddEntry(h_BDT_bkg, "non-proton event (true 0p, reco 0p)", "L");
		h_BDT_signal->Draw("hist");
		h_BDT_bkg->Draw("hist same");
		leg->Draw();
		c->Print(pdfname.c_str());


		// ---------- draw test result ----------------------------------
	    }
	}
    }

    fout->cd();
    h_proton_portion->Write(); h_proton_perev->Write(); h_other_perev->Write(); h_auc->Write();
    c->SetLogy(0);
    Draw2DHist(c, h_proton_portion, pdfname);
    Draw2DHist(c, h_proton_perev, pdfname);
    Draw2DHist(c, h_other_perev, pdfname);
    Draw2DHist(c, h_proton_other_ratio, pdfname);
    Draw2DHist(c, h_auc, pdfname);
    c->Print((pdfname+"]").c_str());


    fout->Close();
    return;
}
