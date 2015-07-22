function acc = kth_test_LED_RBF(path,scale_factor, shift, dim, delta)

gamma = 1/dim*2^delta;

RIEMANNIAN_KERNEL = @(X,Y,gamma) exp( -gamma*( norm(logm(X) - logm(Y),'fro') )^2 );

actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = 0;
real_labels = zeros(n_peo*n_actions);
est_labels  = zeros(n_peo*n_actions);
  

load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );

j=1;
  for pe_ts= 1: n_peo
      
      load_svm_model =strcat( './svm_models/LED-RBF_svm_run_', int2str(pe_ts), '_delta', num2str(delta), '.mat');
      load(load_svm_model); %loading model and X_train

      for act_ts = 1:n_actions
          
          real_labels(j) = act_ts;
          name_load_cov = strcat( load_sub_path, '/cov_', all_people(pe_ts), '_', actions(act_ts), '_dim', int2str(dim), '.h5');
          %char(name_load_cov)         
          hinfo = hdf5info( char(name_load_cov) );
          one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
          X_test(:,:,1) = one_video;
          
          K_test = compute_kernel_svm(X_test,X_train, RIEMANNIAN_KERNEL,gamma);
          [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
          est_labels(j) = predict_label;
          j=j+1;
          
          if predict_label == act_ts
              acc = acc+1;
          end

      end
      
     save_labels = strcat('./svm_results/LED-RBF_scale', int2str(scale_factor), '-shift', int2str(shift),'-delta',num2str(delta),'.mat' );     
     save(save_labels, 'est_labels', 'real_labels', 'delta');
  
  end
  %[acc n_peo n_actions]
  acc = acc*100/(n_peo*n_actions);
   
