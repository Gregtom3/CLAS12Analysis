int test(){
  TH1F *h1 = new TH1F("h1","h1",100,-5,5);
  TH1F *h2 = (TH1F*)h1->Clone();
  h1->Fill(5);
  cout << h1->GetEntries() << endl;
  cout << h2->GetEntries() << endl;
  return 0;

}
