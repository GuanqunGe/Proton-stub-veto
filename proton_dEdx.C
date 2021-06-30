/* The purpose of this macro is to plot the distribution of mean summed ADC per hits for true proton and other particle species
 * Author, Guanqun Ge, March 2021
 */
void proton_dEdx(){

     TFile *fin = new TFile("/pnfs/uboone/persistent/users/markross/Feb2021_gLEE_files/bnb_overlay_combined123_COFredo_withExtra_v43.5.root.deoverlapped.v1.root", "read");

     TTree* t = (TTree*)fin->Get("singlephotonana/vertex_tree");
     std::cout << t->GetEntries() << std::endl;

     std::vector<double>* reco_track_calo_energy_plane2 = 0;
     std::vector<int>* m_reco_track_num_calo_hits_p2 = 0;
     std::vector<int>* sim_track_pdg = 0;
     std::vector<double>* sim_track_overlay_fraction = 0;

     t->SetBranchAddress("reco_track_calo_energy_plane2", &reco_track_calo_energy_plane2);
     t->SetBranchAddress("reco_track_num_calo_hits_plane2", &m_reco_track_num_calo_hits_p2);
     t->SetBranchAddress("sim_track_pdg", &sim_track_pdg);
     t->SetBranchAddress("sim_track_overlay_fraction", &sim_track_overlay_fraction);

     TFile *fout = new TFile("EnergyPerHitPlane2.root", "recreate");
     TH1I *hPDG = new TH1I("hPDG", "hPDG", 300, 0, 3000);
     TH1D *hproton = new TH1D("hproton", "hproton", 140, 0, 700);
     TH1D *hnproton = new TH1D("hnonproton", "hnonproton", 140, 0, 700);

     std::vector<double> gain_data = {230.3, 237.6, 243.7};
     double work_function = 23.6;
     double recombination_factor = 0.62;
     double time_tick = 0.5; //in us

     for(int i=0; i< t->GetEntries(); ++i){
     //for(int i=0; i< 10000; ++i){
        if(i % 5000 == 0) std::cout << "On Entry " << i << "/" << t->GetEntries() << std::endl;
	t->GetEntry(i);
	
	//not every event has reco tracks
	if(reco_track_calo_energy_plane2->size() > 0){
	    //std::cout << reco_track_calo_energy_plane2->at(0)<< " " << m_reco_track_num_calo_hits_p2->at(0) << " " 
	    //	  << sim_track_pdg->at(0) << " " << sim_track_overlay_fraction->at(0) << std::endl;

	    //calc the summed ADC per Hit (round to the nearest integer)
	    double EperHit = reco_track_calo_energy_plane2->at(0) / m_reco_track_num_calo_hits_p2->at(0);
	    double QperHit = EperHit * recombination_factor /(work_function * 1e-6);
	    double IntegralPerHit = QperHit / gain_data[2]; //on plane 2
	    int sumADCperHit = TMath::Nint(IntegralPerHit);  //turns out SummedADC() and Integral() have very similar result
	    //int sumADCperHit = TMath::Nint(IntegralPerHit/time_tick);
	    //std::cout << IntegralPerHit << " " << sumADCperHit << std::endl;
	
	    if(std::isnan(IntegralPerHit) || std::isinf(IntegralPerHit)) continue;

	    hPDG->Fill(sim_track_pdg->at(0));
	    if( sim_track_pdg->at(0) == 2212){
	        if( sim_track_overlay_fraction->at(0) < 0.5)
		    hproton->Fill(sumADCperHit);
	    }
	    else
		hnproton->Fill(sumADCperHit);
	}
     }

     //normalize histograms
     hproton->Scale(1/hproton->Integral());
     hnproton->Scale(1/hnproton->Integral());

     //overlay summedADC per Hit histgorams for proton and non-proton particle

     TCanvas *c = new TCanvas("cADCperHit", "cADCperHit");
     TLegend *leg = new TLegend(0.5, 0.7, 0.89, 0.89);
     leg->SetBorderSize(0);
     gStyle->SetOptStat(0);
     c->Print("ADCperHit.pdf[");
     hproton->SetMaximum(1.1*TMath::Max(hproton->GetBinContent(hproton->GetMaximumBin()), hnproton->GetBinContent(hnproton->GetMaximumBin())));

     hproton->SetTitle("summed ADC per Hit (area normalized); summed ADC; entries");
     hproton->SetFillColorAlpha(kPink, 0.7);
     hnproton->SetFillColorAlpha(kTeal, 0.7);
     leg->SetHeader("Summed ADC per Hit, area normalized", "C");
     leg->AddEntry(hproton, "Proton", "F");
     leg->AddEntry(hnproton, "Everything Non-Proton", "F");
     c->cd();
     hproton->Draw("hist");
     hnproton->Draw("same hist");
     leg->Draw();
     c->Print("ADCperHit.pdf");
     c->Print("ADCperHit.pdf]");
     
          
     fout->cd();
     hPDG->Write();
     hproton->Write();
     hnproton->Write();
     c->Write();

     fout->Close();
     fin->Close();
}
