/* The purpose of this standalone is to plot some truth distributions for proton clusters, and non-proton clusters
 * author Guanqun Ge, March 2021
 */
void cluster(){

    // new file that I ran with updated single photon module (- min ioc of cluster included)
    TFile *fin = new TFile("/uboone/data/users/gge/SinglePhotonAnalysis/vertexed_file/04092021_lowproton_seaview_test_group_cluster/merged_vertexed_singlephoton_hist.root", "read");
 
    // old file that Mark gave me, which doesn't include min ioc of cluster in the TTree
    //TFile *fin = new TFile("/pnfs/uboone/persistent/users/markross/Feb2021_gLEE_files/bnb_overlay_combined123_COFredo_withExtra_v43.5.root.deoverlapped.v1.root", "read");

    TTree *tin = (TTree*)fin->Get("singlephotonana/vertex_tree");

    int cluster_candidate_number;
    int cluster_candidate_group_number;
    std::vector<int>* cluster_candidate_plane = nullptr;
    std::vector<double>* cluster_candidate_PCA = nullptr;
    std::vector<double>* cluster_candidate_mean_ADC = nullptr;
    std::vector<double>* cluster_candidate_min_dist=nullptr;
    std::vector<double>* cluster_candidate_energy=nullptr;
    std::vector<int>* cluster_candidate_matched = nullptr;
    std::vector<int>* cluster_candidate_pdg = nullptr;
    std::vector<double>* cluster_candidate_overlay_fraction = nullptr;
    std::vector<double>* cluster_candidate_min_impact_parameter_to_shower = nullptr;
    std::vector<double>* cluster_candidate_min_conversion_dist_to_shower_start = nullptr;
    std::vector<double>* cluster_candidate_min_ioc_to_shower_start = nullptr;
    std::vector<double>* cluster_candidate_ioc_based_length=nullptr;
    std::vector<double>* cluster_candidate_wire_tick_based_length=nullptr;
    std::vector<double>* cluster_candidate_mean_ADC_first_to_second_ratio=nullptr;
    std::vector<double>* cluster_candidate_track_angle_wrt_shower_direction=nullptr;
    std::vector<double>* cluster_candidate_linear_fit_chi2=nullptr;
    std::vector<double>* cluster_candidate_ADC_RMS=nullptr;
    std::vector<int>*    cluster_candidate_num_in_group = nullptr;
    std::vector<std::vector<int>>* cluster_group_candidate_indices = nullptr;
    std::vector<double>* cluster_candidate_group_timeoverlap_fraction=nullptr;

    //setup branches
    tin->SetBranchAddress("sss_num_candidates", &cluster_candidate_number);
    tin->SetBranchAddress("sss_candidate_plane", &cluster_candidate_plane);
    tin->SetBranchAddress("sss_candidate_PCA", &cluster_candidate_PCA);
    tin->SetBranchAddress("sss_candidate_mean_ADC", &cluster_candidate_mean_ADC);
    tin->SetBranchAddress("sss_candidate_min_dist", &cluster_candidate_min_dist);
    tin->SetBranchAddress("sss_candidate_energy", &cluster_candidate_energy);
    tin->SetBranchAddress("sss_candidate_matched", &cluster_candidate_matched);
    tin->SetBranchAddress("sss_candidate_pdg", &cluster_candidate_pdg);
    tin->SetBranchAddress("sss_candidate_overlay_fraction", &cluster_candidate_overlay_fraction);
    tin->SetBranchAddress("sss_candidate_min_impact_parameter_to_shower", &cluster_candidate_min_impact_parameter_to_shower);
    tin->SetBranchAddress("sss_candidate_min_conversion_dist_to_shower_start", &cluster_candidate_min_conversion_dist_to_shower_start);
    tin->SetBranchAddress("sss_candidate_min_ioc_to_shower_start", &cluster_candidate_min_ioc_to_shower_start);
    tin->SetBranchAddress("sss_candidate_ioc_based_length", &cluster_candidate_ioc_based_length);
    tin->SetBranchAddress("sss_candidate_wire_tick_based_length", &cluster_candidate_wire_tick_based_length);
    tin->SetBranchAddress("sss_candidate_mean_ADC_first_to_second_ratio", &cluster_candidate_mean_ADC_first_to_second_ratio);
    tin->SetBranchAddress("sss_candidate_track_angle_wrt_shower_direction", &cluster_candidate_track_angle_wrt_shower_direction);
    tin->SetBranchAddress("sss_candidate_linear_fit_chi2", &cluster_candidate_linear_fit_chi2);
    tin->SetBranchAddress("sss_candidate_ADC_RMS", &cluster_candidate_ADC_RMS);
    tin->SetBranchAddress("sss_num_candidate_groups", &cluster_candidate_group_number);
    tin->SetBranchAddress("sss_num_candidate_in_group", &cluster_candidate_num_in_group);
    tin->SetBranchAddress("grouped_sss_candidate_indices", &cluster_group_candidate_indices);
    tin->SetBranchAddress("sss_candidate_group_timeoverlap_fraction", &cluster_candidate_group_timeoverlap_fraction);


    TFile *fout = new TFile("TEST.root", "recreate");
    TH1D* hPCA_proton = new TH1D("hPCA_proton", "PCA of cluster (area normalized); PCA; entries", 100, 0.5, 1.0);
    TH1D* hPCA_other = new TH1D("hPCA_other", "hPCA_other", 100, 0.5, 1.0);
    TH1D* hmeanADC_proton = new TH1D("hmeanADC_proton", "mean ADC of cluster (area normalized); mean summed ADC; entries", 100, 0, 1000);
    TH1D* hmeanADC_other = new TH1D("hmeanADC_other", "hmeanADC_other", 100, 0, 1000);
    TH1D* hmindist_proton = new TH1D("hmin_dist_proton", "min dist from cluster to vertex (area normalized); min dist; entries", 200, 0, 200);
    TH1D* hmindist_other = new TH1D("hmin_dist_other", "hmin_dist_other", 200, 0, 200);
    TH1D* henergy_proton = new TH1D("henergy_proton", "energy of cluster (area normalized); energy; entries", 200, 0, 400);
    TH1D* henergy_other= new TH1D("henergy_other", "henergy_other", 200, 0, 400);
    TH1D* hoverlay_proton = new TH1D("hoverlay_proton", "overlay fraction of cluster (area normalized); overlay fraction; entries", 100, 0, 0.5);
    TH1D* hoverlay_other = new TH1D("hoverlay_other", "hoverlay_other", 100, 0, 0.5);
    TH1D* hmin_impact_proton = new TH1D("hmin_impact_proton", "min impact parameter of cluster to shower (area normalized); min impact parameter/cm; entries", 100, 0, 500);
    TH1D* hmin_impact_other = new TH1D("hmin_impact_other", "hmin_impact_other", 100, 0, 500); 
    TH1D* hmin_conv_dist_proton = new TH1D("hmin_conv_dist_proton", "min conv. distance from cluster to shower start (area normalized); min conv. dist./cm; entries", 100, 0, 800);
    TH1D* hmin_conv_dist_other = new TH1D("hmin_conv_dist_other", "hmin_conv_dist_other", 100, 0, 800);
    TH1D* hmin_ioc_proton = new TH1D("hmin_ioc_proton", "min IOC of cluster to shower start (area normalized); min IOC; entries", 100, 0, 1);
    TH1D* hmin_ioc_other = new TH1D("hmin_ioc_other", "hmin_ioc_other", 100,0,1);
    TH1D* hioc_based_length_proton = new TH1D("hioc_based_length_proton", "IOC based cluster length (area normalized); cluster length; entries", 100,0,100); 
    TH1D* hioc_based_length_other= new TH1D("hioc_based_length_other", "hioc_based_length_other", 100,0,100); 
    TH1D* hwire_tick_based_length_proton = new TH1D("hwire_tick_based_length_proton", "Wire-Tick based cluster length (area normalized); cluster length; entries", 100, 0, 100);
    TH1D* hwire_tick_based_length_other = new TH1D("hwire_tick_based_length_other", "hwire_tick_based_length_other", 100, 0, 100);
    TH1D* hmean_ADC_first_to_second_ratio_proton = new TH1D("hmean_ADC_first_to_second_ratio_proton", "mean ADC ratio (first half/second half), area normalized; mean ADC ratio of first/second half; entries", 100, 0, 5);
    TH1D* hmean_ADC_first_to_second_ratio_other = new TH1D("hmean_ADC_first_to_second_ratio_other", "hmean_ADC_first_to_second_ratio_other", 100, 0, 5);
    TH1D* htrack_angle_wrt_shower_direction_proton = new TH1D("htrack_angle_wrt_shower_direction_proton", "angle between cluster and shower direction (area normalized); angle/radian; entries", 100, 0, 3.5);
    TH1D* htrack_angle_wrt_shower_direction_other= new TH1D("htrack_angle_wrt_shower_direction_other", "htrack_angle_wrt_shower_direction_other", 100, 0, 3.5);
    TH1D* hlinear_fit_chi2_proton = new TH1D("hlinear_fit_chi2_proton", "Chi2 from linear fit (area normalized); linear fit #chi^{2}; entries", 100, 0, 5000);
    TH1D* hlinear_fit_chi2_other = new TH1D("hlinear_fit_chi2_other", "hlinear_fit_chi2_other", 100, 0, 5000);
    TH1D* hADC_RMS_proton = new TH1D("hADC_RMS_proton", "RMS of hit ADC (area normalized); ADC RMS; entries", 200, 0, 2000);
    TH1D* hADC_RMS_other = new TH1D("hADC_RMS_other", "hADC_RMS_other", 200, 0, 2000);
   
    
    TH1D* htimeoverlap_2p_proton = new TH1D("htimeoverlap_2p_proton", "Minimum time overlap fraction in 2-cluster groups(area normalized); minimum timeoverlap fraction; entries", 100, 0, 1);
    TH1D* htimeoverlap_2p_other = new TH1D("htimeoverlap_2p_other", "htimeoverlap_2p_other", 100, 0, 1);
    TH1D* htimeoverlap_3p_proton = new TH1D("htimeoverlap_3p_proton", "Minimum time overlap fraction in 3-cluster groups(area normalized); minimum timeoverlap fraction; entries", 50, 0, 1);
    TH1D* htimeoverlap_3p_other = new TH1D("htimeoverlap_3p_other", "htimeoverlap_3p_other", 50, 0, 1);
    TH1D* htimeoverlap_3p_part_proton = new TH1D("htimeoverlap_3p_part_proton", "Minimum time overlap fraction in 3-cluster groups(not pure)(area normalized); minimum timeoverlap fraction; entries", 100, 0, 1);
    TH1D* htimeoverlap_3p_fullother = new TH1D("htimeoverlap_3p_fullother", "htimeoverlap_3p_fullother", 100, 0, 1);

    int total_num_cluster_group = 0;
    int total_proton_cluster_group = 0;
    //for(int nentry=0; nentry != 2000; ++nentry){
    for(int nentry=0; nentry != tin->GetEntries(); ++nentry){

	tin->GetEntry(nentry);
	if( nentry % 10000 == 0) std::cout << "On Entry " << nentry << "/" << tin->GetEntries() << std::endl;
 	
	for(int i=0; i != cluster_candidate_number; ++i){
	    //if this cluster is matched to a proton
	    if(cluster_candidate_matched->at(i) && cluster_candidate_pdg->at(i) == 2212){
		hPCA_proton->Fill(cluster_candidate_PCA->at(i));
		hmeanADC_proton->Fill(cluster_candidate_mean_ADC->at(i));
		hmindist_proton->Fill(cluster_candidate_min_dist->at(i));
		henergy_proton->Fill(cluster_candidate_energy->at(i));
		hoverlay_proton->Fill(cluster_candidate_overlay_fraction->at(i));
		hmin_impact_proton->Fill(cluster_candidate_min_impact_parameter_to_shower->at(i));
		hmin_conv_dist_proton->Fill(cluster_candidate_min_conversion_dist_to_shower_start->at(i));
		hmin_ioc_proton->Fill(cluster_candidate_min_ioc_to_shower_start->at(i));
		hioc_based_length_proton->Fill(cluster_candidate_ioc_based_length->at(i));
		hwire_tick_based_length_proton->Fill(cluster_candidate_wire_tick_based_length->at(i));
		hmean_ADC_first_to_second_ratio_proton->Fill(cluster_candidate_mean_ADC_first_to_second_ratio->at(i));
		htrack_angle_wrt_shower_direction_proton->Fill(cluster_candidate_track_angle_wrt_shower_direction->at(i));		
		hlinear_fit_chi2_proton->Fill(cluster_candidate_linear_fit_chi2->at(i));
		hADC_RMS_proton->Fill(cluster_candidate_ADC_RMS->at(i));	
	    }
	    //if the cluster is not matched to MCP or if it's matched to other particle species
	    else{
		hPCA_other->Fill(cluster_candidate_PCA->at(i));
                hmeanADC_other->Fill(cluster_candidate_mean_ADC->at(i));
                hmindist_other->Fill(cluster_candidate_min_dist->at(i));
                henergy_other->Fill(cluster_candidate_energy->at(i));
                hoverlay_other->Fill(cluster_candidate_overlay_fraction->at(i));	
		hmin_impact_other->Fill(cluster_candidate_min_impact_parameter_to_shower->at(i));
		hmin_conv_dist_other->Fill(cluster_candidate_min_conversion_dist_to_shower_start->at(i));
		hmin_ioc_other->Fill(cluster_candidate_min_ioc_to_shower_start->at(i));
		hioc_based_length_other->Fill(cluster_candidate_ioc_based_length->at(i));
		hwire_tick_based_length_other->Fill(cluster_candidate_wire_tick_based_length->at(i));
		hmean_ADC_first_to_second_ratio_other->Fill(cluster_candidate_mean_ADC_first_to_second_ratio->at(i));
		htrack_angle_wrt_shower_direction_other->Fill(cluster_candidate_track_angle_wrt_shower_direction->at(i));		
		hlinear_fit_chi2_other->Fill(cluster_candidate_linear_fit_chi2->at(i));
		hADC_RMS_other->Fill(cluster_candidate_ADC_RMS->at(i));	
	    }
	}

	//grouped clusters
	total_num_cluster_group += cluster_candidate_group_number;

	for(int i = 0; i!= cluster_candidate_group_number; ++i){
	    auto &cluster_pos = cluster_group_candidate_indices->at(i);
	    bool all_matched_proton = true;
	    for(auto index : cluster_pos){
	        if(!cluster_candidate_matched->at(index) || cluster_candidate_pdg->at(index) != 2212){
		    all_matched_proton = false;
		    break;
	         }
	    }

	    // if all grouped clusters are matched to proton
	    if(all_matched_proton){
		++total_proton_cluster_group;
	     	if(cluster_candidate_num_in_group->at(i) == 2) htimeoverlap_2p_proton->Fill(cluster_candidate_group_timeoverlap_fraction->at(i)); 
	    	else if(cluster_candidate_num_in_group->at(i) == 3) htimeoverlap_3p_proton->Fill(cluster_candidate_group_timeoverlap_fraction->at(i)); 
	    }else{
	    	if(cluster_candidate_num_in_group->at(i) == 2) htimeoverlap_2p_other->Fill(cluster_candidate_group_timeoverlap_fraction->at(i)); 
	    	else if(cluster_candidate_num_in_group->at(i) == 3) htimeoverlap_3p_other->Fill(cluster_candidate_group_timeoverlap_fraction->at(i)); 
	    }


	    bool part_matched_proton = false;
	    for(auto index : cluster_pos)
		if(cluster_candidate_matched->at(index)  && cluster_candidate_pdg->at(index) == 2212){
		   part_matched_proton = true;
		   break;
		}
	    
	    if(part_matched_proton){
		if(cluster_candidate_num_in_group->at(i) == 3) htimeoverlap_3p_part_proton->Fill(cluster_candidate_group_timeoverlap_fraction->at(i));
	    }else{
		if(cluster_candidate_num_in_group->at(i) == 3) htimeoverlap_3p_fullother->Fill(cluster_candidate_group_timeoverlap_fraction->at(i));
	    }
  	}
    }

    std::cout << "In total, there are " << total_num_cluster_group << " number of cluster-groups, of which " << total_proton_cluster_group << " are proton cluster groups"  << std::endl;

    //save address to vector, to simplify plotting
    std::vector<TH1D*> hists_proton{hPCA_proton, hmeanADC_proton, hmindist_proton,henergy_proton, hoverlay_proton, hmin_impact_proton, hmin_conv_dist_proton, hmin_ioc_proton, hioc_based_length_proton, hwire_tick_based_length_proton, hmean_ADC_first_to_second_ratio_proton, htrack_angle_wrt_shower_direction_proton,hlinear_fit_chi2_proton, hADC_RMS_proton, htimeoverlap_2p_proton, htimeoverlap_3p_proton, htimeoverlap_3p_part_proton};
    std::vector<TH1D*> hists_other{hPCA_other, hmeanADC_other, hmindist_other, henergy_other, hoverlay_other, hmin_impact_other, hmin_conv_dist_other, hmin_ioc_other, hioc_based_length_other, hwire_tick_based_length_other, hmean_ADC_first_to_second_ratio_other, htrack_angle_wrt_shower_direction_other,hlinear_fit_chi2_other, hADC_RMS_other, htimeoverlap_2p_other, htimeoverlap_3p_other, htimeoverlap_3p_fullother};
    //area-normalize histograms
    for(auto hp : hists_proton) hp->Scale(1/hp->Integral()); 
    for(auto hp : hists_other) hp->Scale(1/hp->Integral()); 


    //overlay the histograms
    TCanvas *c = new TCanvas("c", "c", 800, 600);
    c->Print("cluster_distribution.pdf[");
    TLegend *leg = new TLegend(0.5, 0.7, 0.89, 0.89);
    leg->SetBorderSize(0);
    gStyle->SetOptStat(0);

    for(int i=0; i!= hists_proton.size(); ++i){
        hists_proton[i]->SetMaximum(1.1*TMath::Max(hists_proton[i]->GetBinContent(hists_proton[i]->GetMaximumBin()), hists_other[i]->GetBinContent(hists_other[i]->GetMaximumBin())));
        hists_proton[i]->SetFillColorAlpha(kPink, 0.7);
        hists_other[i]->SetFillColorAlpha(kTeal, 0.7);

        leg->AddEntry(hists_proton[i], "Proton-matched cluster", "F");
        leg->AddEntry(hists_other[i], "Other clusters", "F");
        c->cd();
        hists_proton[i]->Draw("hist");
        hists_other[i]->Draw("hist same"); 
        leg->Draw();
        c->Update();
        c->Print("cluster_distribution.pdf");

	c->Clear();
 	leg->Clear();
    }
    c->Print("cluster_distribution.pdf]");

    fin->Close();
    fout->Close();
}
