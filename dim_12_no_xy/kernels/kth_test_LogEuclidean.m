function acc = kth_test_LogEuclidean(path,scale_factor, shift, sigma, dim)
% clear all
% scale_factor =1;
% shift =0;
% sigma =1;


RIEMANNIAN_KERNEL = @(X,Y,sigma) exp( -( dist_LogEuclidean(X,Y) )^2/(2*sigma^2) );

actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

%display('Testing svm + Kernel Log-Euclidean Distance');

acc = 0;
real_labels = zeros(n_peo*n_actions);
est_labels  = zeros(n_peo*n_actions);
  

load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );

j=1;
  for pe_ts= 1: n_peo
      
      load_svm_model =strcat( './svm_models/logEucl_svm_run_', int2str(pe_ts), '_Sigma', num2str(sigma), '.mat');
      load(load_svm_model); %loading model and X_train

      for act_ts = 1:n_actions
          
          real_labels(j) = act_ts;
          name_load_cov = strcat( load_sub_path, '/cov_', all_people(pe_ts), '_', actions(act_ts), '_dim', int2str(dim), '.h5');
          %char(name_load_cov)         
          hinfo = hdf5info( char(name_load_cov) );
          one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
          X_test(:,:,1) = one_video;
          
          K_test = compute_kernel_svm(X_test,X_train, RIEMANNIAN_KERNEL,sigma);
          [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
          est_labels(j) = predict_label;
          j=j+1;
          
          if predict_label == act_ts
              acc = acc+1;
          end

      end
      
     save_labels = strcat('./svm_results/LogEucl_scale', int2str(scale_factor), '-shift', int2str(shift),'-sigma',num2str(sigma),'.mat' );     
     save(save_labels, 'est_labels', 'real_labels', 'sigma');
  
  end
  %[acc n_peo n_actions]
  acc = acc*100/(n_peo*n_actions);
   
