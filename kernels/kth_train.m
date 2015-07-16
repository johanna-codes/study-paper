clear all
clc

%/home/johanna/toolbox/libsvm-3.20/matlab --> Wanda


path  = 'home/johanna/codes/codes-git/study-paper/trunk/';
dim = 14; 

actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0; 

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

display('Training svm + Kernel Log-Euclidean Distance');
sc_ss = 

n_test = (n_peo-1)*n_actions;

load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );
  for pe_ts= 1: n_peo
      X_train = zeros(dim,dim,n_test);
      labels = zeros(n_test,1);
      k =1;
      for pe_tr=1: n_peo
          if pe_tr~=pe_ts
              for act=1: n_actions
                 name_load_cov = strcat(load_sub_path, '/LogMcov_', all_people(pe_tr), '_', actions(act), '_dim', int2str(dim), '.h5');
                 hinfo = hdf5info(name_load_cov);
                 one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                 X_train(:,:,k) = one_video;
                 k=k+1;
              end
          end    
      end
      
      %Entrenar aca y guardar para cada peo_test
    
    %load_cov << load_sub_path.str() << "/LogMcov_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
  
  end
   
