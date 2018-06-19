package com.example.mostafa.demo.adapters;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import com.amazonaws.auth.CognitoCachingCredentialsProvider;
import com.amazonaws.regions.Region;
import com.amazonaws.regions.Regions;
import com.amazonaws.services.s3.AmazonS3Client;
import com.bumptech.glide.Glide;
import com.example.mostafa.demo.PreferenceManager;
import com.example.mostafa.demo.R;
import com.example.mostafa.demo.models.Friends;

import java.util.Calendar;
import java.util.List;


public class FriendAdapter extends RecyclerView.Adapter<FriendAdapter.MyViewHolder> {
    private Context mContext;
    private List<Friends> friendsList;
    private AmazonS3Client s3;
    private String TAG = "Homie";

    public class MyViewHolder extends RecyclerView.ViewHolder {
        public TextView title, count;
        public ImageView thumbnail, overflow;
        public ImageButton removeFriend;

        public MyViewHolder(View view) {
            super(view);
            title = (TextView) view.findViewById(R.id.title_friend);
            count = view.findViewById(R.id.count_friend);
            thumbnail = (ImageView) view.findViewById(R.id.thumbnail_friend);
            removeFriend = view.findViewById(R.id.overflow_friend);

        }
    }

    private void credentialProvider() {
        CognitoCachingCredentialsProvider credentialsProvider = new CognitoCachingCredentialsProvider(
                mContext,
                "us-east-1:72e60533-8780-47c4-a4aa-9b4c7b24e0a0", // Identity pool ID
                Regions.US_EAST_1 // Region
        );

        setAmazonS3Client(credentialsProvider);
    }

    private void setAmazonS3Client(CognitoCachingCredentialsProvider amazonS3Client) {
        s3 = new AmazonS3Client(amazonS3Client);
        s3.setRegion(Region.getRegion(Regions.US_EAST_1));
    }

    public FriendAdapter(Context mContext, List<Friends> friendsList) {
        this.mContext = mContext;
        this.friendsList = friendsList;
        credentialProvider();
    }

    @Override
    public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.friendcard, parent, false);

        return new MyViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(final MyViewHolder holder, int position) {
        final Friends friend = friendsList.get(position);
        holder.title.setText(friend.getUserName());
        holder.count.setText(friend.getEmail());

        final Calendar calendar = Calendar.getInstance();
        calendar.add(Calendar.DAY_OF_WEEK, 7);

        String displayPicture = s3.generatePresignedUrl("omessenger-userfiles-mobilehub-792948277/public", friend.getID(), calendar.getTime()).toString();
        Glide.with(mContext)
                .load(displayPicture)
                .placeholder(R.drawable.user)
                .into(holder.thumbnail);
    }

    @Override
    public int getItemCount() {
        return friendsList.size();
    }
}


