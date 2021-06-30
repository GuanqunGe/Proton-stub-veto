/******************************************************************
 * Script that save information of clusters in vertex_tree of every event into a flat tree,
 * with each entry corresponding to one cluster
 * -- Guanqun Ge, March 2021
 */
void flat_root(){

    std::string infilepath="FileLOC";
    TFile *fin = new TFile((infilepath+"merged_vertexed_singlephoton_hist_CONFIGURATION.root").c_str(), "read");
    TTree *tin = (TTree*)fin->Get("singlephotonana/vertex_tree");
    std::cout << "total entries "<< tin->GetEntries() << std::endl;

    /*
    auto ObjArray = tin->GetListOfBranches();
    for(int i = 0; i != ObjArray->GetEntries(); ++i){
	std::string name = ObjArray->At(i)->GetName();
	if(name.find("candidate") != std::string::npos)
	    std::cout << name<< std::endl;
    }
    */

    int run_number;
    int subrun_number;
    int event_number;
    int sss_num_candidates;
    std::vector<int>    *sss_candidate_num_hits = nullptr;
    std::vector<int>    *sss_candidate_num_wires = nullptr;
    std::vector<int>    *sss_candidate_num_ticks = nullptr;
    std::vector<int>    *sss_candidate_plane = nullptr;
    std::vector<double> *sss_candidate_PCA = nullptr;
    std::vector<double> *sss_candidate_mean_ADC = nullptr;
    std::vector<double> *sss_candidate_ADC_RMS = nullptr;
    std::vector<double> *sss_candidate_impact_parameter = nullptr;
    std::vector<double> *sss_candidate_fit_slope = nullptr;
    std::vector<double> *sss_candidate_fit_constant = nullptr;
    std::vector<double> *sss_candidate_mean_tick = nullptr;
    std::vector<double> *sss_candidate_max_tick = nullptr;
    std::vector<double> *sss_candidate_min_tick = nullptr;
    std::vector<double> *sss_candidate_mean_wire = nullptr;
    std::vector<double> *sss_candidate_max_wire = nullptr;
    std::vector<double> *sss_candidate_min_wire = nullptr;
    std::vector<double> *sss_candidate_min_dist = nullptr;
    std::vector<double> *sss_candidate_min_impact_parameter_to_shower = nullptr;
    std::vector<double> *sss_candidate_min_conversion_dist_to_shower_start = nullptr;
    std::vector<double> *sss_candidate_min_ioc_to_shower_start = nullptr;
    std::vector<double> *sss_candidate_ioc_based_length = nullptr;
    std::vector<double> *sss_candidate_wire_tick_based_length = nullptr;
    std::vector<double> *sss_candidate_mean_ADC_first_half = nullptr;
    std::vector<double> *sss_candidate_mean_ADC_second_half = nullptr;
    std::vector<double> *sss_candidate_mean_ADC_first_to_second_ratio = nullptr;
    std::vector<double> *sss_candidate_track_angle_wrt_shower_direction = nullptr;
    std::vector<double> *sss_candidate_linear_fit_chi2 = nullptr;
    std::vector<double> *sss_candidate_energy = nullptr;
    std::vector<double> *sss_candidate_angle_to_shower = nullptr;
    std::vector<int>    *sss_candidate_remerge = nullptr;
    std::vector<int>    *sss_candidate_matched = nullptr;
    std::vector<int>    *sss_candidate_pdg = nullptr;
    std::vector<int>    *sss_candidate_parent_pdg = nullptr;
    std::vector<int>    *sss_candidate_trackid = nullptr;
    std::vector<double> *sss_candidate_overlay_fraction = nullptr;
    std::vector<double> *sss_candidate_matched_energy_fraction_best_plane = nullptr;

    tin->SetBranchAddress("run_number", &run_number);
    tin->SetBranchAddress("subrun_number", &subrun_number);
    tin->SetBranchAddress("event_number", &event_number);
    tin->SetBranchAddress("sss_num_candidates", &sss_num_candidates);
    tin->SetBranchAddress("sss_candidate_num_hits", &sss_candidate_num_hits);
    tin->SetBranchAddress("sss_candidate_num_wires", &sss_candidate_num_wires);
    tin->SetBranchAddress("sss_candidate_num_ticks", &sss_candidate_num_ticks);
    tin->SetBranchAddress("sss_candidate_plane", &sss_candidate_plane);
    tin->SetBranchAddress("sss_candidate_PCA", &sss_candidate_PCA);
    tin->SetBranchAddress("sss_candidate_mean_ADC", &sss_candidate_mean_ADC);
    tin->SetBranchAddress("sss_candidate_ADC_RMS", &sss_candidate_ADC_RMS); 
    tin->SetBranchAddress("sss_candidate_impact_parameter", &sss_candidate_impact_parameter); 
    tin->SetBranchAddress("sss_candidate_fit_slope", &sss_candidate_fit_slope); 
    tin->SetBranchAddress("sss_candidate_fit_constant", &sss_candidate_fit_constant); 
    tin->SetBranchAddress("sss_candidate_mean_tick", &sss_candidate_mean_tick); 
    tin->SetBranchAddress("sss_candidate_max_tick", &sss_candidate_max_tick); 
    tin->SetBranchAddress("sss_candidate_min_tick", &sss_candidate_min_tick); 
    tin->SetBranchAddress("sss_candidate_mean_wire", &sss_candidate_mean_wire); 
    tin->SetBranchAddress("sss_candidate_max_wire", &sss_candidate_max_wire); 
    tin->SetBranchAddress("sss_candidate_min_wire", &sss_candidate_min_wire); 
    tin->SetBranchAddress("sss_candidate_min_dist", &sss_candidate_min_dist); 
    tin->SetBranchAddress("sss_candidate_min_impact_parameter_to_shower", &sss_candidate_min_impact_parameter_to_shower); 
    tin->SetBranchAddress("sss_candidate_min_conversion_dist_to_shower_start", &sss_candidate_min_conversion_dist_to_shower_start); 
    tin->SetBranchAddress("sss_candidate_min_ioc_to_shower_start", &sss_candidate_min_ioc_to_shower_start); 
    tin->SetBranchAddress("sss_candidate_ioc_based_length", &sss_candidate_ioc_based_length); 
    tin->SetBranchAddress("sss_candidate_wire_tick_based_length", &sss_candidate_wire_tick_based_length); 
    tin->SetBranchAddress("sss_candidate_mean_ADC_first_half", &sss_candidate_mean_ADC_first_half); 
    tin->SetBranchAddress("sss_candidate_mean_ADC_second_half", &sss_candidate_mean_ADC_second_half); 
    tin->SetBranchAddress("sss_candidate_mean_ADC_first_to_second_ratio", &sss_candidate_mean_ADC_first_to_second_ratio); 
    tin->SetBranchAddress("sss_candidate_track_angle_wrt_shower_direction", &sss_candidate_track_angle_wrt_shower_direction); 
    tin->SetBranchAddress("sss_candidate_linear_fit_chi2", &sss_candidate_linear_fit_chi2); 
    tin->SetBranchAddress("sss_candidate_energy", &sss_candidate_energy); 
    tin->SetBranchAddress("sss_candidate_angle_to_shower", &sss_candidate_angle_to_shower);
    tin->SetBranchAddress("sss_candidate_remerge", &sss_candidate_remerge);
    tin->SetBranchAddress("sss_candidate_matched", &sss_candidate_matched);
    tin->SetBranchAddress("sss_candidate_pdg", &sss_candidate_pdg);
    tin->SetBranchAddress("sss_candidate_parent_pdg", &sss_candidate_parent_pdg);
    tin->SetBranchAddress("sss_candidate_trackid", &sss_candidate_trackid);
    tin->SetBranchAddress("sss_candidate_overlay_fraction", &sss_candidate_overlay_fraction);
    tin->SetBranchAddress("sss_candidate_matched_energy_fraction_best_plane", &sss_candidate_matched_energy_fraction_best_plane);


    //output flat root 
    TFile *fout = new TFile((infilepath+"merged_vertexed_tree_flat_clusters_CONFIGURATION.root").c_str(), "recreate");
    TTree *tout = new TTree("vertex_tree", "vertex_tree");
    TTree *dummy_eventweight = new TTree("eventweight_tree", "eventweight_tree");
    TTree *dummy_pot = new TTree("pot_tree", "pot_tree");
    TTree *dummy_subrun = new TTree("run_subrun_tree", "run_subrun_tree");
    TTree *dummy_slice = new TTree("ncdelta_slice_tree", "ncdelta_slice_tree");
     
    int    num_hits;
    int    num_wires;
    int    num_ticks;
    int    plane;
    double PCA;
    double mean_ADC;
    double ADC_RMS;
    double impact_parameter;
    double fit_slope;
    double fit_constant;
    double mean_tick;
    double max_tick;
    double min_tick;
    double mean_wire;
    double max_wire;
    double min_wire;
    double min_dist;
    double min_impact_parameter_to_shower;
    double min_conversion_dist_to_shower_start;
    double min_ioc_to_shower_start;
    double ioc_based_length;
    double wire_tick_based_length;
    double mean_ADC_first_half;
    double mean_ADC_second_half;
    double mean_ADC_first_to_second_ratio;
    double track_angle_wrt_shower_direction;
    double linear_fit_chi2;
    double energy;
    double angle_to_shower;
    int    remerge;
    int    matched;
    int    pdg;
    int    parent_pdg;
    int    trackid;
    double overlay_fraction;
    double matched_energy_fraction_best_plane; 

    tout->Branch("run_number", &run_number);
    tout->Branch("subrun_number", &subrun_number);
    tout->Branch("event_number", &event_number);
    tout->Branch("num_hits", &num_hits);
    tout->Branch("num_wires", &num_wires);
    tout->Branch("num_ticks", &num_ticks);
    tout->Branch("plane", &plane);
    tout->Branch("PCA", &PCA);
    tout->Branch("mean_ADC", &mean_ADC);
    tout->Branch("ADC_RMS", &ADC_RMS);
    tout->Branch("impact_parameter", &impact_parameter);
    tout->Branch("fit_slope", &fit_slope);
    tout->Branch("fit_constant", &fit_constant);
    tout->Branch("mean_tick", &mean_tick);
    tout->Branch("max_tick", &max_tick);
    tout->Branch("min_tick", &min_tick);
    tout->Branch("mean_wire", &mean_wire);
    tout->Branch("max_wire", &max_wire);
    tout->Branch("min_wire", &min_wire);
    tout->Branch("min_dist", &min_dist);
    tout->Branch("min_impact_parameter_to_shower", &min_impact_parameter_to_shower);
    tout->Branch("min_conversion_dist_to_shower_start", &min_conversion_dist_to_shower_start);
    tout->Branch("min_ioc_to_shower_start", &min_ioc_to_shower_start);
    tout->Branch("ioc_based_length", &ioc_based_length);
    tout->Branch("wire_tick_based_length", &wire_tick_based_length);
    tout->Branch("mean_ADC_first_half", &mean_ADC_first_half);
    tout->Branch("mean_ADC_second_half", &mean_ADC_second_half);
    tout->Branch("mean_ADC_first_to_second_ratio", &mean_ADC_first_to_second_ratio);
    tout->Branch("track_angle_wrt_shower_direction", &track_angle_wrt_shower_direction);
    tout->Branch("linear_fit_chi2", &linear_fit_chi2);
    tout->Branch("energy", &energy);
    tout->Branch("angle_to_shower", &angle_to_shower);
    tout->Branch("remerge", &remerge);
    tout->Branch("matched", &matched);
    tout->Branch("pdg", &pdg);
    tout->Branch("parent_pdg", &parent_pdg);
    tout->Branch("trackid", &trackid);
    tout->Branch("overlay_fraction", &overlay_fraction);
    tout->Branch("matched_energy_fraction_best_plane", &matched_energy_fraction_best_plane);

    int num_cluster = 0;
    //for(int i = 0; i!= 100; ++i){
    for(int i = 0; i!= tin->GetEntries(); ++i){
	tin->GetEntry(i);
	for(int j = 0; j != sss_num_candidates; ++j){
	    ++num_cluster;
	    if( num_cluster % 50000 == 0) std::cout << "On cluster " << num_cluster << std::endl;
    	    num_hits = sss_candidate_num_hits->at(j);
            num_wires = sss_candidate_num_wires->at(j);
            num_ticks = sss_candidate_num_ticks->at(j);
            plane = sss_candidate_plane->at(j);
            PCA = sss_candidate_PCA->at(j);
     	    mean_ADC = sss_candidate_mean_ADC->at(j);
    	    ADC_RMS = sss_candidate_ADC_RMS->at(j);
    	    impact_parameter = sss_candidate_impact_parameter->at(j);
    	    fit_slope = sss_candidate_fit_slope->at(j);
    	    fit_constant = sss_candidate_fit_constant->at(j);
    	    mean_tick = sss_candidate_mean_tick->at(j);
    	    max_tick = sss_candidate_max_tick->at(j);
    	    min_tick = sss_candidate_min_tick->at(j);
    	    mean_wire = sss_candidate_mean_wire->at(j);
    	    max_wire = sss_candidate_max_wire->at(j);
    	    min_wire = sss_candidate_min_wire->at(j);
    	    min_dist = sss_candidate_min_dist->at(j);
    	    min_impact_parameter_to_shower = sss_candidate_min_impact_parameter_to_shower->at(j);
    	    min_conversion_dist_to_shower_start = sss_candidate_min_conversion_dist_to_shower_start->at(j);
    	    min_ioc_to_shower_start = sss_candidate_min_ioc_to_shower_start->at(j);
    	    ioc_based_length = sss_candidate_ioc_based_length->at(j);
    	    wire_tick_based_length = sss_candidate_wire_tick_based_length->at(j);
    	    mean_ADC_first_half = sss_candidate_mean_ADC_first_half->at(j);
    	    mean_ADC_second_half = sss_candidate_mean_ADC_second_half->at(j);
    	    mean_ADC_first_to_second_ratio = sss_candidate_mean_ADC_first_to_second_ratio->at(j);
    	    track_angle_wrt_shower_direction = sss_candidate_track_angle_wrt_shower_direction->at(j);
    	    linear_fit_chi2 = sss_candidate_linear_fit_chi2->at(j);
    	    energy = sss_candidate_energy->at(j);
   	    angle_to_shower = sss_candidate_angle_to_shower->at(j);
    	    remerge = sss_candidate_remerge->at(j);
    	    matched = sss_candidate_matched->at(j);
    	    pdg = sss_candidate_pdg->at(j);
    	    parent_pdg = sss_candidate_parent_pdg->at(j);
    	    trackid = sss_candidate_trackid->at(j);
    	    overlay_fraction = sss_candidate_overlay_fraction->at(j);
    	    matched_energy_fraction_best_plane = sss_candidate_matched_energy_fraction_best_plane->at(j);	    
	    tout->Fill();
	}
    }

    fout->cd();
    tout->Write();
    dummy_eventweight->Write();
    dummy_pot->Write();
    dummy_subrun->Write();
    dummy_slice->Write();
    fout->Close();

    fin->Close();
}
